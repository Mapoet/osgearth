/* osgEarth
 * Copyright 2025 Pelican Mapping
 * MIT License
 */
#include "Ellipsoid"
#include <osgEarth/Notify>
#include <osg/CoordinateSystemNode>

using namespace osgEarth;

#define EM (*static_cast<osg::EllipsoidModel*>(_em.get()))

Ellipsoid::Ellipsoid()
{
    _em = new osg::EllipsoidModel();
}

Ellipsoid::Ellipsoid(double er, double pr)
{
    _em = new osg::EllipsoidModel();
    set(er, pr);
}

Ellipsoid::Ellipsoid(const Ellipsoid& rhs) :
    _ellipsoidToUnitSphere(rhs._ellipsoidToUnitSphere),
    _unitSphereToEllipsoid(rhs._unitSphereToEllipsoid),
    _name(rhs._name),
    _em(new osg::EllipsoidModel(
        rhs.getRadiusEquator(), rhs.getRadiusPolar()))
{
    //nop
}

Ellipsoid::~Ellipsoid()
{
    //nop
}

void
Ellipsoid::setSemiMajorAxis(double value)
{
    set(value, EM.getRadiusPolar());
}

double
Ellipsoid::getSemiMajorAxis() const
{
    return EM.getRadiusEquator();
}

void
Ellipsoid::setSemiMinorAxis(double value)
{
    set(EM.getRadiusEquator(), value);
}

double
Ellipsoid::getSemiMinorAxis() const
{
    return EM.getRadiusPolar();
}

osg::Matrix
Ellipsoid::geocentricToLocalToWorld(const osg::Vec3d& geoc) const
{
    osg::Matrix m;
    EM.computeLocalToWorldTransformFromXYZ(geoc.x(), geoc.y(), geoc.z(), m);
    return m;
}

osg::Vec3d
Ellipsoid::geocentricToUpVector(const osg::Vec3d& geoc) const
{
    return EM.computeLocalUpVector(geoc.x(), geoc.y(), geoc.z());
}

osg::Matrix
Ellipsoid::geodeticToCoordFrame(const osg::Vec3d& lla) const
{
    osg::Matrix m;
    EM.computeCoordinateFrame(
        osg::DegreesToRadians(lla.y()),
        osg::DegreesToRadians(lla.x()),
        m);
    return m;
}

osg::Vec3d
Ellipsoid::geodeticToGeocentric(const osg::Vec3d& lla) const
{
    osg::Vec3d out;
    EM.convertLatLongHeightToXYZ(
        osg::DegreesToRadians(lla.y()),
        osg::DegreesToRadians(lla.x()),
        lla.z(),
        out.x(), out.y(), out.z());
    return out;
}

osg::Vec3d
Ellipsoid::geocentricToGeodetic(const osg::Vec3d& xyz) const
{
    double lat_rad, lon_rad, height_m;
    EM.convertXYZToLatLongHeight(
        xyz.x(), xyz.y(), xyz.z(),
        lat_rad, lon_rad, height_m);

    osg::Vec3d out(
        osg::RadiansToDegrees(lon_rad),
        osg::RadiansToDegrees(lat_rad),
        height_m);

    for (int i = 0; i < 3; ++i)
        if (std::isnan(out[i]))
            out[i] = 0.0;

    return out;
}

void
Ellipsoid::set(double er, double pr)
{
    OE_SOFT_ASSERT_AND_RETURN(er > 0.0 && pr > 0.0, void());
    EM.setRadiusEquator(er);
    EM.setRadiusPolar(pr);

    _ellipsoidToUnitSphere.makeScale(1.0 / er, 1.0 / er, 1.0 / pr);
    _unitSphereToEllipsoid.makeScale(er, er, pr);
}

double
Ellipsoid::longitudinalDegreesToMeters(double value, double lat_deg) const
{
    return value * (2.0 * osg::PI * getRadiusEquator() / 360.0) * cos(osg::DegreesToRadians(lat_deg));
}

double
Ellipsoid::metersToLongitudinalDegrees(double value, double lat_deg) const
{
    auto m = longitudinalDegreesToMeters(1.0, lat_deg);
    if (osg::equivalent(m, 0.0))
        return 0;
    else
        return value / m;
}

bool
Ellipsoid::intersectGeocentricLine(
    const osg::Vec3d& p0_world,
    const osg::Vec3d& p1_world,
    osg::Vec3d& out_world) const
{
    double dist2 = 0.0;
    osg::Vec3d v;
    osg::Vec3d p0 = p0_world * _ellipsoidToUnitSphere;
    osg::Vec3d p1 = p1_world * _ellipsoidToUnitSphere;

    constexpr double R = 1.0; // for unit sphere.

    // http://paulbourke.net/geometry/circlesphere/index.html#linesphere

    osg::Vec3d d = p1 - p0;

    double A = d * d;
    double B = 2.0 * (d * p0);
    double C = (p0 * p0) - R * R;

    // now solve the quadratic A + B*t + C*t^2 = 0.
    double D = B * B - 4.0*A*C;
    if (D > 0)
    {
        // two roots (line passes through sphere twice)
        // find the closer of the two.
        double sqrtD = sqrt(D);
        double t0 = (-B + sqrtD) / (2.0*A);
        double t1 = (-B - sqrtD) / (2.0*A);

        //seg; pick closest:
        if (fabs(t0) < fabs(t1))
            v = d * t0;
        else
            v = d * t1;
    }
    else if (D == 0.0)
    {
        // one root (line is tangent to sphere?)
        double t = -B / (2.0*A);
        v = d * t;
    }

    dist2 = v.length2();

    if (dist2 > 0.0)
    {
        out_world = (p0 + v) * _unitSphereToEllipsoid;
        return true;
    }
    else
    {
        // either no intersection, or the distance was not the max.
        return false;
    }
}

double
Ellipsoid::geodesicDistance(const osg::Vec2d& p1, const osg::Vec2d& p2) const
{
    double
        lat1 = osg::DegreesToRadians(p1.y()),
        lon1 = osg::DegreesToRadians(p1.x()),
        lat2 = osg::DegreesToRadians(p2.y()),
        lon2 = osg::DegreesToRadians(p2.x());

    double Re = getRadiusEquator();
    double Rp = getRadiusPolar();
    double F = (Re - Rp) / Re; // flattening

    double B1 = atan((1.0 - F)*tan(lat1));
    double B2 = atan((1.0 - F)*tan(lat2));

    double P = (B1 + B2) / 2.0;
    double Q = (B2 - B1) / 2.0;

    double G = acos(sin(B1)*sin(B2) + cos(B1)*cos(B2)*cos(fabs(lon2 - lon1)));

    double
        sinG = sin(G),
        sinP = sin(P), sinQ = sin(Q),
        cosP = cos(P), cosQ = cos(Q),
        sinG2 = sin(G / 2.0), cosG2 = cos(G / 2.0);

    double X = (G - sinG)*((sinP*sinP*cosQ*cosQ) / (cosG2*cosG2));
    double Y = (G + sinG)*((cosP*cosP*sinQ*sinQ) / (sinG2*sinG2));

    double dist = Re * (G - (F / 2.0)*(X + Y));

    // NaN could mean start/end points are the same
    return std::isnan(dist) ? 0.0 : dist;
}

osg::Vec3d
Ellipsoid::geodesicInterpolate(const osg::Vec3d& lla1_deg, const osg::Vec3d& lla2_deg, double t) const
{
    double deltaZ = lla2_deg.z() - lla1_deg.z();

    auto w1 = geodeticToGeocentric(lla1_deg) * _ellipsoidToUnitSphere;
    w1.normalize();

    auto w2 = geodeticToGeocentric(lla2_deg) * _ellipsoidToUnitSphere;
    w2.normalize();

    // Geometric slerp in unit sphere space
    // https://en.wikipedia.org/wiki/Slerp#Geometric_Slerp
    double dp = w1 * w2;
    if (dp == 1.0)
    {
        return lla1_deg;
    }

    double angle = acos(dp);

    double s = sin(angle);
    if (s == 0.0)
    {
        return lla1_deg;
    }

    double c1 = sin((1.0 - t)*angle) / s;
    double c2 = sin(t*angle) / s;

    auto n = w1 * c1 + w2 * c2;

    // convert back to world space and apply altitude lerp
    n = n * _unitSphereToEllipsoid;

    auto output = geocentricToGeodetic(n);
    output.z() = lla1_deg.z() + t * deltaZ;
    return output;
}

osg::Vec3d
Ellipsoid::calculateHorizonCullingPoint(const std::vector<osg::Vec3d>& world_points) const
{
    // see: https://cesium.com/blog/2013/05/09/computing-the-horizon-occlusion-point

    OE_SOFT_ASSERT_AND_RETURN(!world_points.empty(), {});

    double max_magnitude = 0.0;

    osg::Vec3d culling_point_dir;
    std::vector<osg::Vec3d> points;
    points.reserve(world_points.size());
    for (auto& world_point : world_points)
    {
        // convert to unit-sphere frame:
        points.emplace_back(world_point * _ellipsoidToUnitSphere);
        culling_point_dir += points.back();
    }
    culling_point_dir.normalize();

    for (auto& point : points)
    {
        auto mag2 = point.length2();
        auto mag = sqrt(mag2);
        auto point_dir = point / mag;
       
        // clamp to ellipsoid
        mag2 = std::max(1.0, mag2);
        mag = std::max(1.0, mag);
       
        auto cos_alpha = point_dir * culling_point_dir;
        auto sin_alpha = (point_dir ^ culling_point_dir).length();
        auto cos_beta = 1.0 / mag;
        auto sin_beta = sqrt(mag2 - 1.0) * cos_beta;

        auto culling_point_mag = 1.0 / (cos_alpha * cos_beta - sin_alpha * sin_beta);

        max_magnitude = std::max(max_magnitude, culling_point_mag);
    }

    auto unit_culling_point = culling_point_dir * max_magnitude;

    // convert back to world frame:
    return unit_culling_point * _unitSphereToEllipsoid;
}
