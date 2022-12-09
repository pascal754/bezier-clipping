# Intersection algorithm for two B-spline curves

## Find intersection points between two B-spline curves using Bezier clipping

- m = n + p + 1 should be satisfied for a curve
(m: knot vector, n: control point, p: degree of curve)

## Limitation

- Not for overlapping curves or self intersection

## With Debug checked

- print verbose debug information

- to save to a file: command > output.txt may be preferred

## Development Tools

- Microsoft Visual Studio Community 2022 (64-bit) - Preview
Version 17.4.0 Preview 2.0
C++20

- imgui-sfml 2.5#1 via vcpkg
