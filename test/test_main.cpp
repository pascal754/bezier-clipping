import Bspline;
import MakeCurve;
import std;

int main()
{
  try
  {
    find_intersection_batch();
  }
  catch (const std::exception& e)
  {
    std::println(std::cerr, "{}", e.what());
  }
  catch (...)
  {
    std::println(std::cerr, "other error");
  }
}
