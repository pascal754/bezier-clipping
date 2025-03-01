//module;
//
//#include "pch.h"

export module NodeInfo;

export enum class ReturnInfo
{
  Default,
  Found,
  DecomposedC1,
  DecomposedC2,
  DecomposedC3,
  Outside,
  Switched,
  Maximum,
  NegativeDelta,
  TooSmallDelta,
  NotClipped,
  NaN,
  Inf,
};

export struct CurveInfo
{
  int id;
  double u1;
  double u2;
};

export struct NodeInfo
{
  int iterationNum;
  int parentNum;
  CurveInfo curveA;
  CurveInfo curveB;
  ReturnInfo rInfo;
  int depth;
};
