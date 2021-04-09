#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

void CheckEqual(double left, double right) {
	if (fabs(left - right) > 1e-6) {
		throw std::runtime_error("Wrong answer: " + to_string(left));
	}
}

struct Point {
	double x_;
	double y_;
	Point() : x_(0), y_(0){};
	Point(double x, double y) : x_(x), y_(y) {}

	friend double GetDistance(Point first, Point second) {
		return sqrt((first.x_ - second.x_) * (first.x_ - second.x_) +
					(first.y_ - second.y_) * (first.y_ - second.y_));
	}
};

class Shape {
public:
	virtual double Perimeter() const = 0;
	virtual double Area() const = 0;
};

class Circle : public Shape {
	Point center_;
	double radius_;

public:
	Circle() = delete;
	Circle(Point center, double radius) : center_(center), radius_(radius){};
	Circle(double radius) : radius_(radius) {
		center_.x_ = 0;
		center_.y_ = 0;
	}

	double Perimeter() const override { return 2 * M_PI * radius_; }
	double Area() const override { return M_PI * radius_ * radius_; }
};

double RouteLine(Point first, Point second, Point third);

bool Sign(double first, double second) {
	if (first * second > 0) return true;
	if (first == 0 && second == 0) return true;
	return false;
}

class Polygon : public Shape {
private:
	friend double RouteLine(Point first, Point second, Point third) {
		Point ab, bc;
		ab.x_ = second.x_ - first.x_;
		ab.y_ = second.y_ - first.y_;
		bc.x_ = third.x_ - second.x_;
		bc.y_ = third.y_ - second.y_;
		return ab.x_ * bc.y_ - ab.y_ * bc.x_;
	}

protected:
	vector<Point> space_;

public:
	Polygon() = delete;
	Polygon(vector<Point> space) : space_(space){};
	double Perimeter() const override {
		double perimeter = 0;
		for (int i = 0; i < size(space_); ++i) {
			if (i == size(space_) - 1) {
				perimeter = perimeter + GetDistance(space_[i], space_[0]);
				break;
			}
			perimeter = perimeter + GetDistance(space_[i], space_[i + 1]);
		}
		return perimeter;
	}

	virtual double Area() const override {
		double square = 0;
		double firstcord = 0;
		double secondcord = 0;

		for (int i = 0; i < size(space_); i++) {
			if (i == size(space_) - 1) {
				firstcord = firstcord + space_[i].x_ * space_[0].y_;
				secondcord = secondcord - space_[0].x_ * space_[i].y_;
				break;
			}
			firstcord = firstcord + space_[i].x_ * space_[i + 1].y_;
			secondcord = secondcord + space_[i + 1].x_ * space_[i].y_;
		}
		square = 0.5 * fabs(firstcord - secondcord);
	return square;
	}

	bool IsConvex() const {
		int i = 1;
		vector<double> vec;
		for (; i < size(space_) - 1; i++) {
			vec.push_back(RouteLine(space_[i - 1], space_[i], space_[i + 1]));
		}
		vec.push_back(RouteLine(space_[i - 2], space_[i - 1], space_[0]));
		vec.push_back(RouteLine(space_[i], space_[0], space_[1]));
		for (int i = 0; i < size(vec); i++) {
			if (!Sign(vec[i], vec[i + 1])) return false;
		}
		return true;
	}
};

class Triangle : public Polygon {
public:
	Triangle(vector<Point> vertex) : Polygon(vertex) {
		if (size(space_) != 3) throw runtime_error("not 3 side");
	};
};

class Square : public Shape {
	double side_;

public:
	Square() = delete;
	Square(double side) : side_(side){};
	double Perimeter() const override { return 4 * side_; }
	double Area() const override { return side_ * side_; }
};

int main() {
	Triangle* triangle = new Triangle({{0, 0}, {0, 1}, {1, 0}});
	Polygon* polygon = new Polygon({{0, 0}, {0, 1}, {1, 1}, {1, 0}});
	Polygon* test = new Polygon({{0.5, 0.5}, {0, 1}, {1, 1}, {1, 0}, {0, 0}});
	Polygon* test2 = new Polygon({{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0.5, 0.5}});
	cout << test->IsConvex() << endl;
	cout << test2->IsConvex();

	{  // inheritance test
		static_cast<Shape*>(new Square(5));
		static_cast<Shape*>(new Circle(5));
		static_cast<Shape*>(polygon);
		static_cast<Shape*>(triangle);
		static_cast<Polygon*>(triangle);
	}

	{  // area test
		CheckEqual(Square(2.5).Area(), 6.25);
		CheckEqual(Circle(1).Area(), acos(-1.0));
		CheckEqual(triangle->Area(), 0.5);
		CheckEqual(polygon->Area(), 1.0);
	}

	{  // perimeter test
		CheckEqual(Square(1.5).Perimeter(), 6);
		CheckEqual(Circle(2).Perimeter(), 4 * acos(-1.0));
		CheckEqual(triangle->Perimeter(), 2 + sqrt(2));
		CheckEqual(polygon->Perimeter(), 4);
	}

	{  // test that triangle throws exception in case of wrong number of vertices
		try {
			try {
				Triangle({{0, 0}, {0, 1}});
			} catch (...) {
				throw std::runtime_error("not 3 side");
			}
			throw "No exception for triangle";
		} catch (const std::runtime_error&) {
		}
	}
}
