#include "header.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

LineSegment::LineSegment() : x_(0.0), y_(0.0) {}

LineSegment::LineSegment(double x, double y) : x_(x), y_(y) {
    assert(std::isfinite(x) && std::isfinite(y));
}

bool LineSegment::set_x(double x) {
    if (!std::isfinite(x)) return false;
    x_ = x;
    return true;
}

bool LineSegment::set_y(double y) {
    if (!std::isfinite(y)) return false;
    y_ = y;
    return true;
}

std::optional<LineSegment> LineSegment::Intersect(
    const LineSegment& other) const {
    if (!IsValid() || !other.IsValid()) return std::nullopt;

    const double left = std::max(x_, other.x_);
    const double right = std::min(y_, other.y_);
    if (left > right) return std::nullopt;
    return LineSegment(left, right);
}

LineSegment LineSegment::operator!() const {
    if (IsValid()) return *this;
    // Здесь x > y.
    if (y_ >= 0.0) return LineSegment(0.0, y_);  // Достаточно обнулить x.
    if (x_ <= 0.0) return LineSegment(x_, 0.0);  // Достаточно обнулить y.
    return LineSegment(0.0, 0.0);                // x > 0 и y < 0.
}

LineSegment::operator int() const {
    const double truncated = std::trunc(y_);
    if (truncated >= std::numeric_limits<int>::max()) {
        return std::numeric_limits<int>::max();
    }
    if (truncated <= std::numeric_limits<int>::min()) {
        return std::numeric_limits<int>::min();
    }
    return static_cast<int>(truncated);
}

LineSegment::operator double() const { return x_; }

LineSegment LineSegment::operator+(int d) const {
    const double new_x = x_ + d;
    return LineSegment(std::min(new_x, y_), y_);
}

bool LineSegment::operator>(const LineSegment& other) const {
    if (!IsValid() || !other.IsValid()) return false;
    return x_ <= other.x_ && other.y_ <= y_;
}

std::ostream& operator<<(std::ostream& os, const LineSegment& segment) {
    return os << "[" << segment.x_ << "; " << segment.y_ << "]";
}