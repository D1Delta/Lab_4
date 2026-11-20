#ifndef LAB_4_HEADER_H
#define LAB_4_HEADER_H

#ifndef LAB4_LINE_SEGMENT_H_
#define LAB4_LINE_SEGMENT_H_

#include <iostream>
#include <optional>

// Отрезок на координатной прямой: x - координата начала, y - координата конца.
//
// Отрезок с x <= y называется правильным диапазоном. Объект с x > y
// допускается (например, его можно получить сеттерами): это неправильный
// диапазон, который исправляется оператором !.
class LineSegment {
 public:
  // Конструктор по умолчанию: отрезок [0; 0].
  LineSegment();

  // Конструктор с параметрами. Значения x и y должны быть конечными числами.
  LineSegment(double x, double y);

  // Конструктор копирования.
  LineSegment(const LineSegment& other) = default;

  LineSegment& operator=(const LineSegment& other) = default;

  // Свойства. Сеттеры возвращают false и не меняют значение, если переданное
  // число не является конечным (NaN или бесконечность).
  double x() const { return x_; }
  double y() const { return y_; }
  bool set_x(double x);
  bool set_y(double y);

  // Возвращает true, если отрезок - правильный диапазон (x <= y).
  bool IsValid() const { return x_ <= y_; }

  // Задание 1. Пересечение отрезков (отрезки замкнутые). Если отрезки не
  // пересекаются (или хотя бы один из них - неправильный диапазон), возвращает
  // std::nullopt, то есть "null".
  std::optional<LineSegment> Intersect(const LineSegment& other) const;

  // Задание 2.

  // Унарная операция !: возвращает правильный диапазон. Если отрезок уже
  // правильный, возвращается его копия. Иначе в 0 устанавливается та
  // координата, из-за которой диапазон неправильный (сначала пробуем x, затем
  // y). Если одной координатой не обойтись (x > 0 и y < 0), обе становятся 0.
  LineSegment operator!() const;

  // Приведение к int (неявное): целая часть координаты y. Значения вне
  // диапазона int ограничиваются границами int.
  operator int() const;

  // Приведение к double (явное): координата x.
  explicit operator double() const;

  // Увеличивает координату x на d. Если после этого x оказывается правее y,
  // отрезок вырождается в точку [y; y], чтобы результат был правильным
  // диапазоном.
  LineSegment operator+(int d) const;

  // Возвращает true, если левый диапазон включает правый (нестрого).
  // Для неправильных диапазонов всегда false.
  bool operator>(const LineSegment& other) const;

  // Вывод полей в поток в виде "[x; y]".
  friend std::ostream& operator<<(std::ostream& os,
                                  const LineSegment& segment);

 private:
  double x_;
  double y_;
};

#endif  // LAB4_LINE_SEGMENT_H_

#endif //LAB_4_HEADER_H
