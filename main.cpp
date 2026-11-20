#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <string>

#include "header.h"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

// Включает вывод кириллицы (UTF-8) в консоли Windows. На других системах
// ничего не делает.
void SetUpConsole() {
#ifdef _WIN32
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif
}

const char* YesNo(bool value) { return value ? "да" : "нет"; }

void PrintHeader(const std::string& title) {
  const std::string line(64, '=');
  std::cout << "\n" << line << "\n" << title << "\n" << line << "\n";
}

void PrintSegment(const std::string& name, const LineSegment& segment) {
  std::cout << "  " << name << " = " << segment;
  if (!segment.IsValid()) std::cout << "  (неправильный диапазон: x > y)";
  std::cout << "\n";
}

// Выводит приглашение и читает строку. При конце ввода завершает программу.
std::string ReadLine(const std::string& prompt) {
  std::cout << prompt;
  std::string line;
  if (!std::getline(std::cin, line)) {
    std::cout << "\nВвод завершён. Выход из программы.\n";
    std::exit(EXIT_SUCCESS);
  }
  return line;
}

// Преобразует всю строку в конечное число типа double. Допускается запятая
// в качестве десятичного разделителя. Возвращает false, если строка не
// является числом.
bool ParseDouble(std::string text, double* value) {
  std::replace(text.begin(), text.end(), ',', '.');
  const char* begin = text.c_str();
  char* end = nullptr;
  errno = 0;
  const double result = std::strtod(begin, &end);
  if (end == begin || errno == ERANGE || !std::isfinite(result)) return false;
  while (std::isspace(static_cast<unsigned char>(*end))) ++end;
  if (*end != '\0') return false;
  *value = result;
  return true;
}

// Читает вещественное число, пока пользователь не введёт корректное.
double ReadDouble(const std::string& prompt) {
  while (true) {
    double value = 0.0;
    if (ParseDouble(ReadLine(prompt), &value)) return value;
    std::cout << "  Ошибка: нужно ввести число (например, -2.5).\n";
  }
}

// Читает целое число из диапазона [min_value; max_value].
int ReadInt(const std::string& prompt, int min_value, int max_value) {
  while (true) {
    const std::string text = ReadLine(prompt);
    const char* begin = text.c_str();
    char* end = nullptr;
    errno = 0;
    const long value = std::strtol(begin, &end, 10);
    const bool is_number = end != begin && errno != ERANGE;
    while (std::isspace(static_cast<unsigned char>(*end))) ++end;
    if (is_number && *end == '\0' && value >= min_value &&
        value <= max_value) {
      return static_cast<int>(value);
    }
    std::cout << "  Ошибка: нужно ввести целое число от " << min_value
              << " до " << max_value << ".\n";
  }
}

// Читает отрезок с клавиатуры. Если allow_incorrect == false, требует x <= y.
LineSegment ReadSegment(const std::string& name, bool allow_incorrect) {
  while (true) {
    std::cout << "Введите отрезок " << name << ":\n";
    const double x = ReadDouble("  начало x = ");
    const double y = ReadDouble("  конец  y = ");
    if (allow_incorrect || x <= y) return LineSegment(x, y);
    std::cout << "  Ошибка: начало x не должно быть больше конца y. "
                 "Повторите ввод.\n";
  }
}

void RunTask1() {
  PrintHeader("Задание 1. Конструкторы, свойства, вывод, пересечение");

  std::cout << "\n1) Конструктор по умолчанию\n";
  const LineSegment default_segment;
  PrintSegment("D", default_segment);

  std::cout << "\n2) Конструктор с параметрами\n";
  const LineSegment a = ReadSegment("A", /*allow_incorrect=*/false);
  PrintSegment("A", a);

  std::cout << "\n3) Конструктор копирования\n";
  LineSegment b(a);
  PrintSegment("B (копия A)", b);

  std::cout << "\n4) Свойства (геттеры и сеттеры), изменяем копию B\n";
  std::cout << "  Сейчас: x = " << b.x() << ", y = " << b.y() << "\n";
  const double new_x = ReadDouble("  Новое x = ");
  const double new_y = ReadDouble("  Новое y = ");
  if (b.set_x(new_x) && b.set_y(new_y)) {
    PrintSegment("B после изменения", b);
    PrintSegment("A (не изменился)", a);
  } else {
    std::cout << "  Ошибка: значение не принято сеттером.\n";
  }

  std::cout << "\n5) Пересечение отрезков A и C\n";
  const LineSegment c = ReadSegment("C", /*allow_incorrect=*/false);
  const std::optional<LineSegment> common = a.Intersect(c);
  std::cout << "  A = " << a << ", C = " << c << "\n";
  if (common.has_value()) {
    std::cout << "  Пересечение = " << *common << "\n";
  } else {
    std::cout << "  Пересечение = null (отрезки не пересекаются)\n";
  }
}

void RunTask2() {
  PrintHeader("Задание 2. Перегруженные операции");
  std::cout << "(Здесь можно вводить и неправильный диапазон, x > y.)\n";

  const LineSegment s = ReadSegment("S", /*allow_incorrect=*/true);
  PrintSegment("S", s);

  std::cout << "\n1) Унарная операция !S (получить правильный диапазон)\n";
  PrintSegment("!S", !s);

  std::cout << "\n2) Приведение к int (неявное): целая часть y\n";
  const int integer_part = s;
  std::cout << "  int(S) = " << integer_part << "\n";

  std::cout << "\n3) Приведение к double (явное): координата x\n";
  const double x_value = static_cast<double>(s);
  std::cout << "  double(S) = " << x_value << "\n";

  std::cout << "\n4) Бинарная операция S + d (увеличение координаты x)\n";
  const int d = ReadInt("  Введите целое d = ",
                        std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max());
  PrintSegment("S + d", s + d);

  std::cout << "\n5) Бинарная операция > (левый диапазон включает правый)\n";
  const LineSegment t = ReadSegment("T", /*allow_incorrect=*/true);
  PrintSegment("T", t);
  std::cout << "  S > T (S включает T): " << YesNo(s > t) << "\n";
  std::cout << "  T > S (T включает S): " << YesNo(t > s) << "\n";
}

}  // namespace

int main() {
  SetUpConsole();
  while (true) {
    PrintHeader("Лабораторная работа №4. Перегрузка операторов (вариант 16)");
    std::cout << "  1 - Задание 1 (класс LineSegment, пересечение отрезков)\n"
              << "  2 - Задание 2 (перегруженные операции)\n"
              << "  0 - Выход\n";
    switch (ReadInt("Ваш выбор: ", 0, 2)) {
      case 1:
        RunTask1();
        break;
      case 2:
        RunTask2();
        break;
      default:
        std::cout << "До свидания!\n";
        return 0;
    }
  }
}