#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <sstream>

using namespace std;

#include "show.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

//#include <CGAL/Triangulation_3.h>
//#include <CGAL/draw_triangulation_3.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/draw_point_set_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Point_set_3<Point> Point_set;


int filter(string s, vector<string> *elements)
{

    //BUG : First element is ' ' :/
    int indicator = 0;
    string element;
    bool in_flag = false, added = false;

    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != ' ' && s[i] != '\t')
        {
            added = false;
            in_flag =true;
            element += s[i];
            
        }
        else
        {
            in_flag = false;
        }
        
        if (!in_flag && !added)
        {
            added = true;
            elements->push_back(element);
            indicator ++;
            element.clear();
        }
    }
    elements->push_back(element);
    indicator ++;
    return indicator;
    
}

std::string extractLastNChars(std::string const &str, int n)
{
    if (str.size() < n) {
        return str;
    }
 
    return str.substr(str.size() - n);
}


void print_point_set (const Point_set& point_set)
{
  cout << "Content of point set:" << endl;
  for (Point_set::const_iterator it = point_set.begin(); it != point_set.end(); ++it)
    std::cout << "* Point " << *it << " : " << point_set.point(*it) << endl;
}


bool read_file(string file_name, Point_set *point_set, vector<Point> *points)
{
    vector<string> elements;
    cout << "Reading file " << file_name << endl;
    if (extractLastNChars(file_name, 4) == ".obj")
    {
        //system("f3d box.obj --up +Z");
        fstream file;
        file.open(file_name, ios::in);

        string inp;
        char del = '\r';

        //First line (info)
        getline(file, inp, del);
        cout << inp;
        //Second line (Date and Time of file creation)
        getline(file, inp, del);
        cout << inp;

        cout << "\n=======================================" << endl;

        stringstream stream;
        long double ac_x, ac_y, ac_z;
        //vector<Point> points;

        int number_of_points=0;
        
        while (getline(file, inp, del))
        {
            elements.clear();
            filter(inp,&elements);      //filter function is not working properly
            //check if readed line is vertex (Starts with v)
            if (elements[0] == "\nv")
            {
                number_of_points ++;
                stream << elements[1];
                stream >> ac_x;
                stream.clear();
                stream << elements[2];
                stream >> ac_y;
                stream.clear();
                stream << elements[3];
                stream >> ac_z;
                stream.clear();
                points->push_back(Point(ac_x, ac_y, ac_z));
                point_set->insert(Point(ac_x, ac_y, ac_z));
                //cout << "\np" << number_of_points << " = " << ac_x << "\t" << ac_y << "\t" << ac_z << endl;
            }
            
        }
        return true;
    }
    else
    {
        cout << "\nUnsupported file format" << endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        cout << "Enter file path as an argument!" << endl;
        return 1;
    }

    Point_set points;
    vector<Point> point_vector;
    read_file(argv[1], &points, &point_vector);

    //cout << "\nVERTEX : " << point_vector[1].x() << " " << point_vector[1].y() << " " << point_vector[1].z() << endl ; 

    print_point_set(points);

    CGAL::draw(points);


}
