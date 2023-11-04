# Intersection algorithm for two B-spline curves using Bezier clipping

## Find intersection points between two B-spline curves

- find intersection between one of (point, line, or curve) and one of (point, line, or curve)
- each iteration info saved in node_info.log
- verbose info optionally saved in calc.log
- m = n + p + 1 should be satisfied for a curve
(m: knot vector [u_0, u_m], n: control points [P_0, P_n], p: degree of curve)

## Limitation

- Not for self intersection

## Development Tools

- Microsoft Visual Studio Community 2022 (64-bit) - 
Version 17.8.0 Preview 6.0
C++latest(modules)

## Library via vcpkg

- sfml 2.6
- imgui 1.89.9
- imgui-sfml 2.6
- magic-enum 0.9.3

## Sample calculation
![screenshot](Screenshot_3.png)
