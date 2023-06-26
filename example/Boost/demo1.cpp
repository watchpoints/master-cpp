#include <iostream>
#include <boost/geometry.hpp>

namespace bg = boost::geometry;

template <typename P>
void test_ring(std::string const& wkt,
    std::size_t expected_main_count,
    std::size_t expected_interior_ring_count,
    std::size_t expected_first_interior_count)
{
    typedef bg::model::polygon<P> the_polygon;
    typedef typename bg::ring_type<the_polygon>::type the_ring;
    typedef typename bg::interior_return_type<the_polygon const>::type the_interior;

    the_polygon poly;
    bg::read_wkt(wkt, poly);

    the_ring ext = bg::exterior_ring(poly);
    the_interior rings = bg::interior_rings(poly);

    std::size_t num_interior_rings = bg::num_interior_rings(poly);
    std::size_t rings_size = boost::size(rings);
    std::size_t ext_size = boost::size(ext);

    std::cout << "Number of interior rings: " << num_interior_rings << std::endl;
    std::cout << "Size of rings: " << rings_size << std::endl;
    std::cout << "Size of exterior ring: " << ext_size << std::endl;

    if (rings_size > 0)
    {
        std::size_t first_ring_size = boost::size(rings.front());
        std::cout << "Size of first interior ring: " << first_ring_size << std::endl;
    }
    
    std::cout << std::endl;

    if (num_interior_rings == expected_interior_ring_count &&
        rings_size == expected_interior_ring_count &&
        ext_size == expected_main_count)
    {
        std::cout << "Test Passed" << std::endl;
    }
    else
    {
        std::cout << "Test Failed" << std::endl;
    }
}

template <typename P>
void test_all()
{
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0),(1 1,1 2,2 2,2 1,1 1))", 5, 1, 5);
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0),(1 1,2 2,2 1,1 1),(1 1,1 2,2 2,1 1))", 5, 2, 4);
    test_ring<P>("POLYGON((0 0,0 3,3 3,3 0,0 0))", 5, 0, 0);
}

int main()
{
    typedef bg::model::d2::point_xy<double> Point;
    test_all<Point>();
    
    return 0;
}
