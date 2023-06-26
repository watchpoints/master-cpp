#include <iostream>
#include <boost/geometry.hpp>

namespace bg = boost::geometry;

typedef bg::model::point<double, 2, bg::cs::cartesian> Point;
typedef bg::model::polygon<Point> Polygon;
typedef typename bg::ring_type<Polygon>::type Ring;
typedef typename bg::interior_return_type<Polygon const>::type InteriorRings;

int main()
{
    std::string wkt = "POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))";
    
    Polygon poly;
    bg::read_wkt(wkt, poly);
    
    Ring exteriorRing = bg::exterior_ring(poly);
    InteriorRings interiorRings = bg::interior_rings(poly);
    
    std::cout << "Exterior Ring:" << std::endl;
    for (const auto& point : exteriorRing)
    {
        std::cout << "(" << bg::get<0>(point) << ", " << bg::get<1>(point) << ")" << std::endl;
    }
    
    std::cout << "Interior Rings:" << std::endl;
    for (const auto& ring : interiorRings)
    {
        for (const auto& point : ring)
        {
            std::cout << "(" << bg::get<0>(point) << ", " << bg::get<1>(point) << ")" << std::endl;
        }
        std::cout << "----" << std::endl;
    }
    
    return 0;
}
