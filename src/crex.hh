#ifndef OB_CREX_HH
#define OB_CREX_HH

#include <string>

namespace OB
{
class Crex
{
public:

  Crex();
  ~Crex();

private:

  std::string flgs;
  std::string rx;
  std::string str;
  bool _color {true};

}; // class Crex
} // namespace OB

#endif // OB_CREX_HH
