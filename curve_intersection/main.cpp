// September 3, 2022
// by SM
// curve intersection

#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include <SFML/Graphics.hpp>

import Bspline;
import Point;

void printUsage()
{
	std::cout << "Usage: command e | b\n";
}

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

void make_curve24(Bspline& curve1, Bspline& curve2)
{
	// two points, degree is 1
	curve1.addPointAndKnots(Point{ 550, 200 });
	curve1.addPointAndKnots(Point{ 550, 200 });

	curve2.addPointAndKnots(Point{ 300, 200 });
	curve2.addPointAndKnots(Point{ 300, 200 });
}

void make_curve25(Bspline& curve1, Bspline& curve2)
{
	// same overlapping closed curves
	curve1.addPointAndKnots(Point{ 200, 210 });
	curve1.addPointAndKnots(Point{ 200, 310 });
	curve1.addPointAndKnots(Point{ 300, 310 });
	curve1.addPointAndKnots(Point{ 200, 210 });

	curve2.addPointAndKnots(Point{ 210, 210 });
	curve2.addPointAndKnots(Point{ 210, 310 });
	curve2.addPointAndKnots(Point{ 310, 310 });
	curve2.addPointAndKnots(Point{ 210, 210 });
}

void do_test100()
{
	Bspline curve1{ 3 };

	curve1.addPointAndKnots(Point{ 200, 200 });
	curve1.addPointAndKnots(Point{ 200, 200 });
	curve1.addPointAndKnots(Point{ 200, 200 });
	curve1.addPointAndKnots(Point{ 200, 200 });
	curve1.findConvexHull();

	curve1.printInfo();

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
	dCurve1->curvePoint(0.0, testPt);
	dCurve1->curvePoint(0.3, testPt);
	dCurve1->curvePoint(0.5, testPt);

	dCurve1 = curve1.decompose(0.5, 0.5);
	dCurve1->curvePoint(0.5, testPt);

	dCurve1 = curve1.decompose(1.0, 1.0);
}



int main()
{
	try
	{
		int decomp_num{}; // number of iteration for intersection

		Bspline curve1{ 3 }, curve2{ 3 };
		std::vector<Point> ptList;
		std::vector<Bspline> splines;
		
		make_curve02(curve1, curve2);

		//std::chrono::steady_clock::time_point start, stop;

		//start = std::chrono::high_resolution_clock::now();

		curve1.findIntersection(curve2, ptList, decomp_num, false);
		//curve1.bezierIntersection(curve2, ptList, decomp_num, false);
			
		//stop = std::chrono::high_resolution_clock::now();
		/*auto duration = duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << '\t' << duration.count() << " ms\n";*/


		std::cout << '\t' << decomp_num << " decomposition(s)\n";

		if (ptList.size() == 0)
		{
			std::cout << "No intersection\n";
		}
		else
		{
			for (int i{}; i < ptList.size(); ++i)
			{
				std::cout << std::format("***intersection point #{}: ", i + 1);
				std::cout << ptList[i] << '\n';
			}
		}

	
		sf::RenderWindow window(sf::VideoMode(800, 800), "Curve Intersection", sf::Style::Titlebar | sf::Style::Close);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			//curve1.drawControlPolygon(window);
			//curve1.drawConvexHull(window, sf::Color::Yellow);
			curve1.drawCurve(window, sf::Color::Green);

			//curve2.drawControlPolygon(window);
			//curve2.drawConvexHull(window, sf::Color::Red);
			curve2.drawCurve(window, sf::Color::Yellow);


			

			auto windowSize{ window.getSize() };

			for (auto& p : ptList)
			{
				sf::CircleShape c{ 3 };
				c.setPosition(static_cast<float>(p.x) - c.getRadius(), windowSize.y - static_cast<float>(p.y) - c.getRadius());
				window.draw(c);
			}

			window.display();

			sf::sleep(sf::milliseconds(100));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch (...)
	{
		std::cerr << "something wrong\n";
		return 1;
	}
}
