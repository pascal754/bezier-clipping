module;

#include "pch.h"
#include <conio.h>

module MakeCurve;

import Point;
import Bspline;
import std;

void make_curve01(Bspline& curve1, Bspline& curve2)
{
  //two closed curves
  curve1.addPointAndKnots(Point(25, 182));
  curve1.addPointAndKnots(Point(41, 182));
  curve1.addPointAndKnots(Point(44, 194));
  curve1.addPointAndKnots(Point(34, 204));
  curve1.addPointAndKnots(Point(33, 218));
  curve1.addPointAndKnots(Point(43, 232));
  curve1.addPointAndKnots(Point(60, 232));
  curve1.addPointAndKnots(Point(76, 221));
  curve1.addPointAndKnots(Point(77, 208));
  curve1.addPointAndKnots(Point(69, 199));
  curve1.addPointAndKnots(Point(63, 189));
  curve1.addPointAndKnots(Point(68, 181));
  curve1.addPointAndKnots(Point(79, 178));
  curve1.addPointAndKnots(Point(87, 184));
  curve1.addPointAndKnots(Point(92, 199));
  curve1.addPointAndKnots(Point(99, 213));
  curve1.addPointAndKnots(Point(118, 212));
  curve1.addPointAndKnots(Point(130, 200));
  curve1.addPointAndKnots(Point(134, 187));
  curve1.addPointAndKnots(Point(127, 177));
  curve1.addPointAndKnots(Point(114, 176));
  curve1.addPointAndKnots(Point(102, 172));
  curve1.addPointAndKnots(Point(94, 166));
  curve1.addPointAndKnots(Point(90, 158));
  curve1.addPointAndKnots(Point(96, 148));
  curve1.addPointAndKnots(Point(107, 145));
  curve1.addPointAndKnots(Point(112, 153));
  curve1.addPointAndKnots(Point(120, 160));
  curve1.addPointAndKnots(Point(131, 159));
  curve1.addPointAndKnots(Point(142, 149));
  curve1.addPointAndKnots(Point(147, 132));
  curve1.addPointAndKnots(Point(145, 117));
  curve1.addPointAndKnots(Point(133, 103));
  curve1.addPointAndKnots(Point(122, 100));
  curve1.addPointAndKnots(Point(111, 109));
  curve1.addPointAndKnots(Point(102, 121));
  curve1.addPointAndKnots(Point(92, 122));
  curve1.addPointAndKnots(Point(88, 114));
  curve1.addPointAndKnots(Point(86, 106));
  curve1.addPointAndKnots(Point(91, 99));
  curve1.addPointAndKnots(Point(105, 92));
  curve1.addPointAndKnots(Point(115, 75));
  curve1.addPointAndKnots(Point(111, 61));
  curve1.addPointAndKnots(Point(93, 49));
  curve1.addPointAndKnots(Point(80, 51));
  curve1.addPointAndKnots(Point(69, 59));
  curve1.addPointAndKnots(Point(66, 73));
  curve1.addPointAndKnots(Point(68, 85));
  curve1.addPointAndKnots(Point(63, 95));
  curve1.addPointAndKnots(Point(52, 100));
  curve1.addPointAndKnots(Point(44, 98));
  curve1.addPointAndKnots(Point(37, 91));
  curve1.addPointAndKnots(Point(39, 73));
  curve1.addPointAndKnots(Point(39, 59));
  curve1.addPointAndKnots(Point(25, 47));
  curve1.addPointAndKnots(Point(14, 56));
  curve1.addPointAndKnots(Point(10, 68));
  curve1.addPointAndKnots(Point(10, 83));
  curve1.addPointAndKnots(Point(17, 94));
  curve1.addPointAndKnots(Point(26, 103));
  curve1.addPointAndKnots(Point(28, 116));
  curve1.addPointAndKnots(Point(24, 130));
  curve1.addPointAndKnots(Point(21, 142));
  curve1.addPointAndKnots(Point(25, 182));

  curve2.addPointAndKnots(Point(79, 248));
  curve2.addPointAndKnots(Point(69, 242));
  curve2.addPointAndKnots(Point(62, 229));
  curve2.addPointAndKnots(Point(64, 215));
  curve2.addPointAndKnots(Point(74, 202));
  curve2.addPointAndKnots(Point(89, 195));
  curve2.addPointAndKnots(Point(103, 192));
  curve2.addPointAndKnots(Point(118, 192));
  curve2.addPointAndKnots(Point(136, 187));
  curve2.addPointAndKnots(Point(151, 175));
  curve2.addPointAndKnots(Point(159, 154));
  curve2.addPointAndKnots(Point(161, 132));
  curve2.addPointAndKnots(Point(157, 120));
  curve2.addPointAndKnots(Point(143, 119));
  curve2.addPointAndKnots(Point(128, 116));
  curve2.addPointAndKnots(Point(117, 111));
  curve2.addPointAndKnots(Point(103, 103));
  curve2.addPointAndKnots(Point(93, 95));
  curve2.addPointAndKnots(Point(86, 83));
  curve2.addPointAndKnots(Point(81, 64));
  curve2.addPointAndKnots(Point(78, 43));
  curve2.addPointAndKnots(Point(70, 34));
  curve2.addPointAndKnots(Point(57, 29));
  curve2.addPointAndKnots(Point(45, 28));
  curve2.addPointAndKnots(Point(79, 248));
}

void make_curve02(Bspline& curve1, Bspline& curve2)
{
  // two closed curves (bigger)
  curve1.addPointAndKnots(Point(65, 476));
  curve1.addPointAndKnots(Point(113, 476));
  curve1.addPointAndKnots(Point(122, 512));
  curve1.addPointAndKnots(Point(92, 542));
  curve1.addPointAndKnots(Point(89, 584));
  curve1.addPointAndKnots(Point(119, 626));
  curve1.addPointAndKnots(Point(170, 626));
  curve1.addPointAndKnots(Point(218, 593));
  curve1.addPointAndKnots(Point(221, 554));
  curve1.addPointAndKnots(Point(197, 527));
  curve1.addPointAndKnots(Point(179, 497));
  curve1.addPointAndKnots(Point(194, 473));
  curve1.addPointAndKnots(Point(227, 464));
  curve1.addPointAndKnots(Point(251, 482));
  curve1.addPointAndKnots(Point(266, 527));
  curve1.addPointAndKnots(Point(287, 569));
  curve1.addPointAndKnots(Point(344, 566));
  curve1.addPointAndKnots(Point(380, 530));
  curve1.addPointAndKnots(Point(392, 491));
  curve1.addPointAndKnots(Point(371, 461));
  curve1.addPointAndKnots(Point(332, 458));
  curve1.addPointAndKnots(Point(296, 446));
  curve1.addPointAndKnots(Point(272, 428));
  curve1.addPointAndKnots(Point(260, 404));
  curve1.addPointAndKnots(Point(278, 374));
  curve1.addPointAndKnots(Point(311, 365));
  curve1.addPointAndKnots(Point(326, 389));
  curve1.addPointAndKnots(Point(350, 410));
  curve1.addPointAndKnots(Point(383, 407));
  curve1.addPointAndKnots(Point(416, 377));
  curve1.addPointAndKnots(Point(431, 326));
  curve1.addPointAndKnots(Point(425, 281));
  curve1.addPointAndKnots(Point(389, 239));
  curve1.addPointAndKnots(Point(356, 230));
  curve1.addPointAndKnots(Point(323, 257));
  curve1.addPointAndKnots(Point(296, 293));
  curve1.addPointAndKnots(Point(266, 296));
  curve1.addPointAndKnots(Point(254, 272));
  curve1.addPointAndKnots(Point(248, 248));
  curve1.addPointAndKnots(Point(263, 227));
  curve1.addPointAndKnots(Point(305, 206));
  curve1.addPointAndKnots(Point(335, 155));
  curve1.addPointAndKnots(Point(323, 113));
  curve1.addPointAndKnots(Point(269, 77));
  curve1.addPointAndKnots(Point(230, 83));
  curve1.addPointAndKnots(Point(197, 107));
  curve1.addPointAndKnots(Point(188, 149));
  curve1.addPointAndKnots(Point(194, 185));
  curve1.addPointAndKnots(Point(179, 215));
  curve1.addPointAndKnots(Point(146, 230));
  curve1.addPointAndKnots(Point(122, 224));
  curve1.addPointAndKnots(Point(101, 203));
  curve1.addPointAndKnots(Point(107, 149));
  curve1.addPointAndKnots(Point(107, 107));
  curve1.addPointAndKnots(Point(65, 71));
  curve1.addPointAndKnots(Point(32, 98));
  curve1.addPointAndKnots(Point(20, 134));
  curve1.addPointAndKnots(Point(20, 179));
  curve1.addPointAndKnots(Point(41, 212));
  curve1.addPointAndKnots(Point(68, 239));
  curve1.addPointAndKnots(Point(74, 278));
  curve1.addPointAndKnots(Point(62, 320));
  curve1.addPointAndKnots(Point(53, 356));
  curve1.addPointAndKnots(Point(65, 476));

  curve2.addPointAndKnots(Point(157, 704));
  curve2.addPointAndKnots(Point(127, 686));
  curve2.addPointAndKnots(Point(106, 647));
  curve2.addPointAndKnots(Point(112, 605));
  curve2.addPointAndKnots(Point(142, 566));
  curve2.addPointAndKnots(Point(187, 545));
  curve2.addPointAndKnots(Point(229, 536));
  curve2.addPointAndKnots(Point(274, 536));
  curve2.addPointAndKnots(Point(328, 521));
  curve2.addPointAndKnots(Point(373, 485));
  curve2.addPointAndKnots(Point(397, 422));
  curve2.addPointAndKnots(Point(403, 356));
  curve2.addPointAndKnots(Point(391, 320));
  curve2.addPointAndKnots(Point(349, 317));
  curve2.addPointAndKnots(Point(304, 308));
  curve2.addPointAndKnots(Point(271, 293));
  curve2.addPointAndKnots(Point(229, 269));
  curve2.addPointAndKnots(Point(199, 245));
  curve2.addPointAndKnots(Point(178, 209));
  curve2.addPointAndKnots(Point(163, 152));
  curve2.addPointAndKnots(Point(154, 89));
  curve2.addPointAndKnots(Point(130, 62));
  curve2.addPointAndKnots(Point(91, 47));
  curve2.addPointAndKnots(Point(55, 44));
  curve2.addPointAndKnots(Point(157, 704));
}

void make_curve03(Bspline& curve1, Bspline& curve2)
{
  // two curves (one intersection)
  curve1.addPointAndKnots(Point{ 555, 305 });
  curve1.addPointAndKnots(Point{ 465, 245 });
  curve1.addPointAndKnots(Point{ 400, 255 });
  curve1.addPointAndKnots(Point{ 345, 295 });
  curve1.addPointAndKnots(Point{ 330, 365 });
  curve1.addPointAndKnots(Point{ 340, 425 });
  curve1.addPointAndKnots(Point{ 315, 475 });

  curve2.addPointAndKnots(Point{ 465, 475 });
  curve2.addPointAndKnots(Point{ 430, 415 });
  curve2.addPointAndKnots(Point{ 405, 320 });
  curve2.addPointAndKnots(Point{ 390, 215 });
  curve2.addPointAndKnots(Point{ 350, 170 });
  curve2.addPointAndKnots(Point{ 285, 145 });
  curve2.addPointAndKnots(Point{ 225, 140 });
}

void make_curve04(Bspline& curve1, Bspline& curve2)
{
  // almost overlapping(line)
  curve1.addPointAndKnots(Point{ 350, 300 });
  curve1.addPointAndKnots(Point{ 400, 300 });
  curve1.addPointAndKnots(Point{ 450, 300 });
  curve1.addPointAndKnots(Point{ 500, 300 });
  curve1.addPointAndKnots(Point{ 550, 300 });

  curve2.addPointAndKnots(Point{ 350, 300 });
  curve2.addPointAndKnots(Point{ 400, 300 });
  curve2.addPointAndKnots(Point{ 450, 300 });
  curve2.addPointAndKnots(Point{ 500, 300 });
  curve2.addPointAndKnots(Point{ 550, 305 });
}

void make_curve05(Bspline& curve1, Bspline& curve2)
{
  // closed curve and a line sgement (no intersection)
  curve1.addPointAndKnots(Point{ 200, 200 });
  curve1.addPointAndKnots(Point{ 200, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 200, 200 });

  curve2.addPointAndKnots(Point{ 450, 200 });
  curve2.addPointAndKnots(Point{ 450, 250 });
  curve2.addPointAndKnots(Point{ 450, 300 });
  curve2.addPointAndKnots(Point{ 450, 350 });
  curve2.addPointAndKnots(Point{ 450, 400 });
}

void make_curve06(Bspline& curve1, Bspline& curve2)
{
  // a closed curve and a curve (no intersection)
  curve1.addPointAndKnots(Point{ 555, 305 });
  curve1.addPointAndKnots(Point{ 465, 245 });
  curve1.addPointAndKnots(Point{ 400, 255 });
  curve1.addPointAndKnots(Point{ 345, 295 });
  curve1.addPointAndKnots(Point{ 330, 365 });
  curve1.addPointAndKnots(Point{ 340, 425 });
  curve1.addPointAndKnots(Point{ 315, 475 });

  curve2.addPointAndKnots(Point{ 200, 210 });
  curve2.addPointAndKnots(Point{ 200, 310 });
  curve2.addPointAndKnots(Point{ 300, 310 });
  curve2.addPointAndKnots(Point{ 200, 210 });
}

void make_curve07(Bspline& curve1, Bspline& curve2)
{
  // same overlapping closed curves
  curve1.addPointAndKnots(Point{ 200, 210 });
  curve1.addPointAndKnots(Point{ 200, 310 });
  curve1.addPointAndKnots(Point{ 300, 310 });
  curve1.addPointAndKnots(Point{ 200, 210 });

  curve2 = curve1;
}

void make_curve08(Bspline& curve1, Bspline& curve2)
{
  // same overlapping open curves
  curve1.addPointAndKnots(Point{ 555, 305 });
  curve1.addPointAndKnots(Point{ 465, 245 });
  curve1.addPointAndKnots(Point{ 400, 255 });
  curve1.addPointAndKnots(Point{ 345, 295 });
  curve1.addPointAndKnots(Point{ 330, 365 });
  curve1.addPointAndKnots(Point{ 340, 425 });
  curve1.addPointAndKnots(Point{ 315, 475 });

  curve2 = curve1;
}

void make_curve09(Bspline& curve1, Bspline& curve2)
{
  // two same lines
  curve1.addPointAndKnots(Point{ 350, 300 });
  curve1.addPointAndKnots(Point{ 400, 300 });
  curve1.addPointAndKnots(Point{ 450, 300 });
  curve1.addPointAndKnots(Point{ 500, 300 });
  curve1.addPointAndKnots(Point{ 550, 300 });

  curve2 = curve1;
}

void make_curve10(Bspline& curve1, Bspline& curve2)
{
  // two horizontal lines
  curve1.addPointAndKnots(Point{ 150, 300 });
  curve1.addPointAndKnots(Point{ 200, 300 });
  curve1.addPointAndKnots(Point{ 250, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 350, 300 });

  curve2.addPointAndKnots(Point{ 400, 300 });
  curve2.addPointAndKnots(Point{ 450, 300 });
  curve2.addPointAndKnots(Point{ 500, 300 });
  curve2.addPointAndKnots(Point{ 550, 300 });
  curve2.addPointAndKnots(Point{ 600, 300 });
}

void make_curve11(Bspline& curve1, Bspline& curve2)
{
  // two horizontal lines
  curve1.addPointAndKnots(Point{ 150, 300 });
  curve1.addPointAndKnots(Point{ 200, 300 });
  curve1.addPointAndKnots(Point{ 250, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 350, 300 });

  curve2.addPointAndKnots(Point{ 150, 400 });
  curve2.addPointAndKnots(Point{ 200, 400 });
  curve2.addPointAndKnots(Point{ 250, 400 });
  curve2.addPointAndKnots(Point{ 300, 400 });
  curve2.addPointAndKnots(Point{ 350, 400 });
}

void make_curve12(Bspline& curve1, Bspline& curve2)
{
  // two same vertical lines
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 250 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 350 });
  curve1.addPointAndKnots(Point{ 300, 400 });

  curve2 = curve1;
}

void make_curve13(Bspline& curve1, Bspline& curve2)
{
  // two vertically parallel lines
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 250 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 350 });
  curve1.addPointAndKnots(Point{ 300, 400 });

  curve2.addPointAndKnots(Point{ 400, 200 });
  curve2.addPointAndKnots(Point{ 400, 250 });
  curve2.addPointAndKnots(Point{ 400, 300 });
  curve2.addPointAndKnots(Point{ 400, 350 });
  curve2.addPointAndKnots(Point{ 400, 400 });
}

void make_curve14(Bspline& curve1, Bspline& curve2)
{
  // almost overlapping(one line)
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 250 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 350 });
  curve1.addPointAndKnots(Point{ 300, 400 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 250 });
  curve2.addPointAndKnots(Point{ 300, 300 });
  curve2.addPointAndKnots(Point{ 300, 350 });
  curve2.addPointAndKnots(Point{ 305, 400 });
}

void make_curve15(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 250 });
  curve2.addPointAndKnots(Point{ 300, 300 });
  curve2.addPointAndKnots(Point{ 300, 350 });
  curve2.addPointAndKnots(Point{ 300, 400 });
}

void make_curve16(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });
  curve1.addPointAndKnots(Point{ 300, 300 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 250 });
  curve2.addPointAndKnots(Point{ 300, 300 });
  curve2.addPointAndKnots(Point{ 300, 350 });
  curve2.addPointAndKnots(Point{ 300, 400 });
}

void make_curve17(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 300, 400 });
  curve1.addPointAndKnots(Point{ 300, 400 });
  curve1.addPointAndKnots(Point{ 300, 400 });
  curve1.addPointAndKnots(Point{ 300, 400 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 250 });
  curve2.addPointAndKnots(Point{ 300, 300 });
  curve2.addPointAndKnots(Point{ 300, 350 });
  curve2.addPointAndKnots(Point{ 300, 400 });
}

void make_curve18(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 350, 200 });
  curve2.addPointAndKnots(Point{ 400, 200 });
  curve2.addPointAndKnots(Point{ 450, 200 });
  curve2.addPointAndKnots(Point{ 500, 200 });
}

void make_curve19(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 400, 200 });
  curve1.addPointAndKnots(Point{ 400, 200 });
  curve1.addPointAndKnots(Point{ 400, 200 });
  curve1.addPointAndKnots(Point{ 400, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 350, 200 });
  curve2.addPointAndKnots(Point{ 400, 200 });
  curve2.addPointAndKnots(Point{ 450, 200 });
  curve2.addPointAndKnots(Point{ 500, 200 });
}

void make_curve20(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 500, 200 });
  curve1.addPointAndKnots(Point{ 500, 200 });
  curve1.addPointAndKnots(Point{ 500, 200 });
  curve1.addPointAndKnots(Point{ 500, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 350, 200 });
  curve2.addPointAndKnots(Point{ 400, 200 });
  curve2.addPointAndKnots(Point{ 450, 200 });
  curve2.addPointAndKnots(Point{ 500, 200 });
}

void make_curve21(Bspline& curve1, Bspline& curve2)
{
  // a point and a line
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 350, 200 });
  curve2.addPointAndKnots(Point{ 400, 200 });
  curve2.addPointAndKnots(Point{ 450, 200 });
  curve2.addPointAndKnots(Point{ 500, 200 });
}

void make_curve22(Bspline& curve1, Bspline& curve2)
{
  // two points
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
}

void make_curve23(Bspline& curve1, Bspline& curve2)
{
  // two identical points
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });
  curve1.addPointAndKnots(Point{ 300, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
}


// make sure degree of the curves are one
void make_curve24(Bspline& curve1, Bspline& curve2)
{
  // two points, degree is 1
  curve1.changeDegree(1);
  curve2.changeDegree(1);

  curve1.addPointAndKnots(Point{ 550, 200 });
  curve1.addPointAndKnots(Point{ 550, 200 });

  curve2.addPointAndKnots(Point{ 300, 200 });
  curve2.addPointAndKnots(Point{ 300, 200 });
}

void make_curve25(Bspline& curve1, Bspline& curve2)
{
  // same closed curves, offset
  curve1.addPointAndKnots(Point{ 200, 210 });
  curve1.addPointAndKnots(Point{ 200, 310 });
  curve1.addPointAndKnots(Point{ 300, 310 });
  curve1.addPointAndKnots(Point{ 200, 210 });

  curve2.addPointAndKnots(Point{ 210, 210 });
  curve2.addPointAndKnots(Point{ 210, 310 });
  curve2.addPointAndKnots(Point{ 310, 310 });
  curve2.addPointAndKnots(Point{ 210, 210 });
}

void make_curve26(Bspline& curve1, Bspline& curve2)
{
  curve1.addPointAndKnots(Point{ 338, 351 });
  curve1.addPointAndKnots(Point{ 283, 417 });
  curve1.addPointAndKnots(Point{ 146, 473 });
  curve1.addPointAndKnots(Point{ 180, 584 });
  curve1.addPointAndKnots(Point{ 279, 633 });
  curve1.addPointAndKnots(Point{ 383, 611 });
  curve1.addPointAndKnots(Point{ 414, 571 });
  curve1.addPointAndKnots(Point{ 438, 522 });
  curve1.addPointAndKnots(Point{ 456, 464 });
  curve1.addPointAndKnots(Point{ 478, 391 });
  curve1.addPointAndKnots(Point{ 525, 344 });
  curve1.addPointAndKnots(Point{ 589, 336 });
  curve1.addPointAndKnots(Point{ 617, 347 });
  curve1.addPointAndKnots(Point{ 671, 401 });
  curve1.addPointAndKnots(Point{ 684, 503 });
  curve1.addPointAndKnots(Point{ 669, 555 });
  curve1.addPointAndKnots(Point{ 616, 605 });
  curve1.addPointAndKnots(Point{ 570, 636 });
  curve1.addPointAndKnots(Point{ 430, 679 });
  curve1.addPointAndKnots(Point{ 315, 715 });
  curve1.addPointAndKnots(Point{ 237, 742 });
  curve1.addPointAndKnots(Point{ 169, 745 });
  curve1.addPointAndKnots(Point{ 99, 609 });
  curve1.addPointAndKnots(Point{ 81, 484 });
  curve1.addPointAndKnots(Point{ 86, 348 });
  curve1.addPointAndKnots(Point{ 102, 248 });
  curve1.addPointAndKnots(Point{ 142, 215 });
  curve1.addPointAndKnots(Point{ 159, 208 });
  curve1.addPointAndKnots(Point{ 442, 129 });
  curve1.addPointAndKnots(Point{ 444, 236 });
  curve1.addPointAndKnots(Point{ 440, 314 });
  curve1.addPointAndKnots(Point{ 410, 367 });
  curve1.addPointAndKnots(Point{ 356, 428 });
  curve1.addPointAndKnots(Point{ 310, 476 });
  curve1.addPointAndKnots(Point{ 321, 523 });
  curve1.addPointAndKnots(Point{ 363, 523 });
  curve1.addPointAndKnots(Point{ 383, 496 });
  curve1.addPointAndKnots(Point{ 407, 448 });
  curve1.addPointAndKnots(Point{ 445, 393 });
  curve1.addPointAndKnots(Point{ 483, 330 });
  curve1.addPointAndKnots(Point{ 496, 296 });
  curve1.addPointAndKnots(Point{ 526, 261 });
  curve1.addPointAndKnots(Point{ 538, 253 });
  curve1.addPointAndKnots(Point{ 571, 253 });
  curve1.addPointAndKnots(Point{ 662, 274 });
  curve1.addPointAndKnots(Point{ 721, 345 });
  curve1.addPointAndKnots(Point{ 754, 500 });
  curve1.addPointAndKnots(Point{ 745, 591 });
  curve1.addPointAndKnots(Point{ 655, 678 });
  curve1.addPointAndKnots(Point{ 479, 764 });
  curve1.addPointAndKnots(Point{ 336, 776 });
  curve1.addPointAndKnots(Point{ 169, 774 });
  curve1.addPointAndKnots(Point{ 94, 739 });
  curve1.addPointAndKnots(Point{ 52, 586 });
  curve1.addPointAndKnots(Point{ 24, 458 });
  curve1.addPointAndKnots(Point{ 31, 329 });
  curve1.addPointAndKnots(Point{ 40, 221 });
  curve1.addPointAndKnots(Point{ 118, 121 });
  curve1.addPointAndKnots(Point{ 223, 78 });
  curve1.addPointAndKnots(Point{ 317, 60 });
  curve1.addPointAndKnots(Point{ 428, 57 });
  curve1.addPointAndKnots(Point{ 455, 69 });
  curve1.addPointAndKnots(Point{ 501, 164 });
  curve1.addPointAndKnots(Point{ 503, 198 });
  curve1.addPointAndKnots(Point{ 511, 207 });
  curve1.addPointAndKnots(Point{ 552, 202 });
  curve1.addPointAndKnots(Point{ 606, 189 });
  curve1.addPointAndKnots(Point{ 626, 187 });
  curve1.addPointAndKnots(Point{ 667, 198 });
  curve1.addPointAndKnots(Point{ 680, 207 });
  curve1.addPointAndKnots(Point{ 717, 267 });
  curve1.addPointAndKnots(Point{ 763, 346 });
  curve1.addPointAndKnots(Point{ 787, 435 });
  curve1.addPointAndKnots(Point{ 792, 517 });
  curve1.addPointAndKnots(Point{ 776, 652 });
  curve1.addPointAndKnots(Point{ 646, 762 });
  curve1.addPointAndKnots(Point{ 533, 769 });
  curve1.addPointAndKnots(Point{ 444, 656 });
  curve1.addPointAndKnots(Point{ 388, 605 });
  curve1.addPointAndKnots(Point{ 323, 500 });
  curve1.addPointAndKnots(Point{ 273, 412 });
  curve1.addPointAndKnots(Point{ 250, 368 });
  curve1.addPointAndKnots(Point{ 231, 289 });
  curve1.addPointAndKnots(Point{ 209, 219 });
  curve1.addPointAndKnots(Point{ 204, 164 });
  curve1.addPointAndKnots(Point{ 232, 85 });
  curve1.addPointAndKnots(Point{ 244, 71 });
  curve1.addPointAndKnots(Point{ 265, 51 });
  curve1.addPointAndKnots(Point{ 294, 40 });
  curve1.addPointAndKnots(Point{ 364, 30 });
  curve1.addPointAndKnots(Point{ 439, 30 });
  curve1.addPointAndKnots(Point{ 485, 38 });
  curve1.addPointAndKnots(Point{ 527, 90 });
  curve1.addPointAndKnots(Point{ 545, 159 });
  curve1.addPointAndKnots(Point{ 556, 243 });
  curve1.addPointAndKnots(Point{ 563, 383 });
  curve1.addPointAndKnots(Point{ 552, 492 });
  curve1.addPointAndKnots(Point{ 522, 531 });
  curve1.addPointAndKnots(Point{ 495, 564 });
  curve1.addPointAndKnots(Point{ 347, 643 });
  curve1.addPointAndKnots(Point{ 294, 665 });
  curve1.addPointAndKnots(Point{ 236, 670 });
  curve1.addPointAndKnots(Point{ 204, 643 });
  curve1.addPointAndKnots(Point{ 174, 537 });
  curve1.addPointAndKnots(Point{ 129, 382 });
  curve1.addPointAndKnots(Point{ 115, 333 });
  curve1.addPointAndKnots(Point{ 94, 281 });
  curve1.addPointAndKnots(Point{ 79, 252 });
  curve1.addPointAndKnots(Point{ 52, 207 });
  curve1.addPointAndKnots(Point{ 36, 173 });
  curve1.addPointAndKnots(Point{ 50, 105 });
  curve1.addPointAndKnots(Point{ 81, 68 });
  curve1.addPointAndKnots(Point{ 135, 47 });
  curve1.addPointAndKnots(Point{ 278, 10 });
  curve1.addPointAndKnots(Point{ 435, 22 });
  curve1.addPointAndKnots(Point{ 554, 25 });
  curve1.addPointAndKnots(Point{ 597, 60 });
  curve1.addPointAndKnots(Point{ 602, 143 });
  curve1.addPointAndKnots(Point{ 554, 247 });
  curve1.addPointAndKnots(Point{ 497, 309 });
  curve1.addPointAndKnots(Point{ 384, 321 });
  curve1.addPointAndKnots(Point{ 344, 307 });
  curve1.addPointAndKnots(Point{ 321, 287 });
  curve1.addPointAndKnots(Point{ 273, 204 });
  curve1.addPointAndKnots(Point{ 253, 179 });
  curve1.addPointAndKnots(Point{ 221, 161 });
  curve1.addPointAndKnots(Point{ 189, 157 });
  curve1.addPointAndKnots(Point{ 179, 162 });
  curve1.addPointAndKnots(Point{ 152, 188 });
  curve1.addPointAndKnots(Point{ 137, 227 });
  curve1.addPointAndKnots(Point{ 120, 286 });
  curve1.addPointAndKnots(Point{ 115, 323 });
  curve1.addPointAndKnots(Point{ 111, 382 });
  curve1.addPointAndKnots(Point{ 118, 458 });
  curve1.addPointAndKnots(Point{ 118, 490 });
  curve1.addPointAndKnots(Point{ 126, 547 });
  curve1.addPointAndKnots(Point{ 131, 581 });
  curve1.addPointAndKnots(Point{ 111, 621 });
  curve1.addPointAndKnots(Point{ 95, 637 });
  curve1.addPointAndKnots(Point{ 70, 657 });
  curve1.addPointAndKnots(Point{ 52, 684 });
  curve1.addPointAndKnots(Point{ 51, 702 });
  curve1.addPointAndKnots(Point{ 74, 725 });
  curve1.addPointAndKnots(Point{ 106, 743 });
  curve1.addPointAndKnots(Point{ 157, 744 });
  curve1.addPointAndKnots(Point{ 193, 740 });
  curve1.addPointAndKnots(Point{ 234, 721 });
  curve1.addPointAndKnots(Point{ 257, 705 });
  curve1.addPointAndKnots(Point{ 276, 689 });
  curve1.addPointAndKnots(Point{ 333, 659 });
  curve1.addPointAndKnots(Point{ 360, 651 });
  curve1.addPointAndKnots(Point{ 397, 646 });
  curve1.addPointAndKnots(Point{ 417, 646 });
  curve1.addPointAndKnots(Point{ 467, 652 });
  curve1.addPointAndKnots(Point{ 496, 664 });
  curve1.addPointAndKnots(Point{ 540, 686 });
  curve1.addPointAndKnots(Point{ 570, 706 });
  curve1.addPointAndKnots(Point{ 583, 714 });
  curve1.addPointAndKnots(Point{ 626, 756 });
  curve1.addPointAndKnots(Point{ 662, 765 });
  curve1.addPointAndKnots(Point{ 693, 767 });
  curve1.addPointAndKnots(Point{ 702, 764 });
  curve1.addPointAndKnots(Point{ 713, 754 });
  curve1.addPointAndKnots(Point{ 728, 711 });
  curve1.addPointAndKnots(Point{ 705, 644 });
  curve1.addPointAndKnots(Point{ 692, 616 });
  curve1.addPointAndKnots(Point{ 649, 565 });
  curve1.addPointAndKnots(Point{ 629, 542 });
  curve1.addPointAndKnots(Point{ 596, 491 });
  curve1.addPointAndKnots(Point{ 569, 446 });
  curve1.addPointAndKnots(Point{ 533, 395 });
  curve1.addPointAndKnots(Point{ 516, 367 });
  curve1.addPointAndKnots(Point{ 491, 325 });
  curve1.addPointAndKnots(Point{ 455, 262 });
  curve1.addPointAndKnots(Point{ 422, 197 });
  curve1.addPointAndKnots(Point{ 404, 152 });
  curve1.addPointAndKnots(Point{ 387, 99 });
  curve1.addPointAndKnots(Point{ 383, 86 });
  curve1.addPointAndKnots(Point{ 362, 51 });
  curve1.addPointAndKnots(Point{ 334, 31 });
  curve1.addPointAndKnots(Point{ 284, 25 });
  curve1.addPointAndKnots(Point{ 257, 25 });
  curve1.addPointAndKnots(Point{ 248, 28 });
  curve1.addPointAndKnots(Point{ 210, 49 });
  curve1.addPointAndKnots(Point{ 189, 67 });
  curve1.addPointAndKnots(Point{ 168, 91 });
  curve1.addPointAndKnots(Point{ 132, 106 });
  curve1.addPointAndKnots(Point{ 98, 110 });
  curve1.addPointAndKnots(Point{ 51, 111 });
  curve1.addPointAndKnots(Point{ 35, 110 });
  curve1.addPointAndKnots(Point{ 28, 119 });

  curve2.addPointAndKnots(Point{ 294, 116 });
  curve2.addPointAndKnots(Point{ 208, 153 });
  curve2.addPointAndKnots(Point{ 159, 254 });
  curve2.addPointAndKnots(Point{ 150, 396 });
  curve2.addPointAndKnots(Point{ 158, 498 });
  curve2.addPointAndKnots(Point{ 226, 624 });
  curve2.addPointAndKnots(Point{ 374, 703 });
  curve2.addPointAndKnots(Point{ 552, 714 });
  curve2.addPointAndKnots(Point{ 641, 681 });
  curve2.addPointAndKnots(Point{ 673, 612 });
  curve2.addPointAndKnots(Point{ 670, 477 });
  curve2.addPointAndKnots(Point{ 657, 377 });
  curve2.addPointAndKnots(Point{ 620, 262 });
  curve2.addPointAndKnots(Point{ 572, 217 });
  curve2.addPointAndKnots(Point{ 493, 213 });
  curve2.addPointAndKnots(Point{ 400, 224 });
  curve2.addPointAndKnots(Point{ 358, 248 });
  curve2.addPointAndKnots(Point{ 283, 321 });
  curve2.addPointAndKnots(Point{ 277, 436 });
  curve2.addPointAndKnots(Point{ 298, 488 });
  curve2.addPointAndKnots(Point{ 390, 607 });
  curve2.addPointAndKnots(Point{ 511, 624 });
  curve2.addPointAndKnots(Point{ 585, 608 });
  curve2.addPointAndKnots(Point{ 589, 565 });
  curve2.addPointAndKnots(Point{ 583, 478 });
  curve2.addPointAndKnots(Point{ 562, 370 });
  curve2.addPointAndKnots(Point{ 529, 326 });
  curve2.addPointAndKnots(Point{ 505, 320 });
  curve2.addPointAndKnots(Point{ 464, 326 });
  curve2.addPointAndKnots(Point{ 423, 372 });
  curve2.addPointAndKnots(Point{ 424, 450 });
  curve2.addPointAndKnots(Point{ 439, 504 });
  curve2.addPointAndKnots(Point{ 502, 544 });
  curve2.addPointAndKnots(Point{ 529, 520 });
  curve2.addPointAndKnots(Point{ 522, 452 });
  curve2.addPointAndKnots(Point{ 502, 408 });
  curve2.addPointAndKnots(Point{ 491, 405 });
  curve2.addPointAndKnots(Point{ 462, 426 });
  curve2.addPointAndKnots(Point{ 462, 459 });
  curve2.addPointAndKnots(Point{ 479, 480 });
}

void write_dat_files()
{
  std::vector<std::function<void(Bspline&, Bspline&)>> vf{
    make_curve01, make_curve02, make_curve03, make_curve04, make_curve05,
    make_curve06, make_curve07, make_curve08, make_curve09, make_curve10,
    make_curve11, make_curve12, make_curve13, make_curve14, make_curve15,
    make_curve16, make_curve17, make_curve18, make_curve19, make_curve20,
    make_curve21, make_curve22, make_curve23, make_curve24, make_curve25,
    make_curve26 };

  for (auto&& [index, make_crv] : std::views::enumerate(vf))
  {
    Bspline c1{ 3 };
    Bspline c2{ 3 };

    c1.setID(1);
    c1.interpolationMode = false;

    c2.interpolationMode = false;
    c2.setID(2);

    ParamInfo pInfo{};

    make_crv(c1, c2);
    auto filename{ std::format("curves_{:02}.dat", index + 1) };
    savePoints(c1, c2, filename);
  }
}

void decompose_test()
{
  Bspline curve1{ 3 };
  curve1.interpolationMode = false;

  curve1.addPointAndKnots(Point{ 200, 200 });
  curve1.addPointAndKnots(Point{ 200, 200 });
  curve1.addPointAndKnots(Point{ 200, 200 });
  curve1.addPointAndKnots(Point{ 200, 200 });
  curve1.findConvexHull();

  Point testPt{};
  //curve1.curvePoint(-1.5, testPt); // out of range error
  curve1.curvePoint(0.0, testPt); // ok
  curve1.curvePoint(0.5, testPt); // ok
  curve1.curvePoint(1.0, testPt); // ok
  //curve1.curvePoint(1.5, testPt); // out of range error

  std::optional<Bspline> dCurve1{ curve1.decompose(0.0, 0.0001) }; // ok
  if (dCurve1)
  {
    dCurve1->curvePoint(0.0, testPt);
    dCurve1->curvePoint(0.00005, testPt);
    dCurve1->curvePoint(0.0001, testPt);
  }

  dCurve1 = curve1.decompose(0.0, 0.0);
  dCurve1 = curve1.decompose(0.0, 0.5);
  if (dCurve1)
  {
    dCurve1->curvePoint(0.3, testPt);
    dCurve1->curvePoint(0.0, testPt);
    dCurve1->curvePoint(0.5, testPt);
  }

  dCurve1 = curve1.decompose(0.5, 0.5);
  if (dCurve1)
  {
    dCurve1->curvePoint(0.5, testPt);
  }

  dCurve1 = curve1.decompose(1.0, 1.0);
}

void find_intersection_batch()
{
  std::vector<std::string> crv_files;
  const std::filesystem::path pwd{ "." };
  for (auto const& dir_entry : std::filesystem::directory_iterator{ pwd })
  {
    if (dir_entry.is_regular_file())
    {
      auto filename{ dir_entry.path().filename().string() };
      if (filename.ends_with(".dat"))
      {
        crv_files.push_back(std::move(filename));
      }
    }
  }

  std::ofstream summaryFile{ "summary.log" };
  constexpr std::string_view fileFmt{ "{:30}{:15}{:20}{:15}{:20}" };
  std::println(summaryFile, fileFmt, "filename", "iter_num", "intersection_num",
    "iter_num", "intersection_num");
  for (const auto& dat_file : crv_files)
  {
    Bspline c1{ 3 };
    c1.setID(1);
    Bspline c2{ 3 };
    c2.setID(2);

    loadPoints(c1, c2, dat_file);

    ParamInfo pInfo{};
    findIntersection(c1, c2, pInfo);

    ParamInfo pInfo2{};
    pInfo2.decomposeFirst = true;

    findIntersection(c1, c2, pInfo2);

    std::println(summaryFile, fileFmt, dat_file,
      pInfo.iterationNum, pInfo.iPoints.size(),
      pInfo2.iterationNum, pInfo2.iPoints.size());

    /*std::println("\n{} finished.", dat_file);
    std::println("\nPress any key to continue ...");
    _getch();*/
  }
}
