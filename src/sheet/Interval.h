#ifndef INTERVAL_H
#define INTERVAL_H

namespace sheet {
    struct Interval {
        Interval(int value = 0) : value(value) {}
        int value;
		bool operator <(const Interval &b) const { return value < b.value; }
    };
}

#endif