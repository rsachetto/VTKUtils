#ifndef POINT_H
#define POINT_H

struct Point {
	double x, y, z;

	double * toDouble() {
		double *p = (double*) malloc(sizeof(double) * 3);
		p[0] = x;
		p[1] = y;
		p[2] = z;
		return p;
	}
};

struct PointComparer {
	bool operator() (const Point& lhs, const Point& rhs) const {

		if (lhs.y == rhs.y) {
			return lhs.x < rhs.x;
		}
		else if (lhs.x == rhs.x) {
			return lhs.y < rhs.y;
		}
		else if (lhs.y < rhs.y) {
			return true;
		}
		else if (lhs.y > rhs.y) {
			return false;
		}

		return false;
	}
};

#endif
