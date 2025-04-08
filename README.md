# Intersection algorithm for two B-spline curves using Bézier clipping

## Find intersection points between two B-spline curves

- Find intersection between one of (point, straight line, or curve) and one of (point, straight line, or curve).
- Each iteration info is saved in node_info.log file.
- Verbose info optionally can be saved in calc.log file.
- A curve should satisfy m = n + p + 1.
(m: knot vector [u_0, u_m], n: control points [P_0, P_n], p: degree of a curve)
- Overlapping can be found by adjusting a level of accuracy resulting in a lot of binary subdivisions.

## How to use

### Interpolation mode
- A curve is supposed to pass through the selected points.
- Global interpolation is used. A curve becomes distorted as the degree of a curve is elevated.
- The same point cannot be selected successively, which causes an LU decomposition failure.
- The minimum number of interpolation points to make a curve is degree + 2.

### Control points mode
- With the interpolation mode off, the selected points are control points of a curve.
- The minimum number of control points to make a curve is degree + 1.
- A point can be created by selecting the same point at least degree + 1 times.

## Limitation

- Not for self intersection

## Development Tools

- Microsoft Visual Studio Community 2022 (64-bit) - 
Version 17.14.0 Preview 1.1
C++latest(modules)

## Library via vcpkg

- imgui:x64-windows 1.91.8
- imgui-sfml:x64-windows 3.0
- magic-enum:x64-windows 0.9.7 (#include path changed from <magic_enum.hpp> to <magic_enum/magic_enum.hpp>)
- sfml:x64-windows 3.0.0 (breaking changes)

## MSVC Bug

- Warning C5201 is incorrect: [Developer Community](https://developercommunity.visualstudio.com/t/C-modules-and-precompiled-header-incor/10016869)
  
## Sample calculation
![screenshot](Screenshot_3.png)

## Reference
- Paper
  - [Curve intersection using Bézier clipping by T W Sederberg and T Nishita](http://nishitalab.org/user/nis/cdrom/cad/CAGD90Curve.pdf)

- Book
  - 'The NURBS Book' by Les Piegl and Wayne Tiller, publisher: Springer, ISBN 3-540-55069-0
