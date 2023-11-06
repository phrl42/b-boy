#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;


void remove_str(std::string &dest, std::string n)
{
  int pos = dest.find(n);
  while(pos != std::string::npos)
  {
    dest.erase(pos, n.size());
  }
}

int main()
{
  std::fstream file("Opcodes.json");

  json data = json::parse(file);


  std::string opcode = "";

  std::cout << "uint8_t opcodes[256] = \n{" << std::endl;
  for(int y = 0; y <= 15; y++)
  {
    for(int x = 0; x <= 15; x++)
    {
      std::stringstream x_stream;
      std::stringstream y_stream;

      y_stream << std::hex << y;
      x_stream << std::hex << x;

      opcode = "0x" + y_stream.str() + x_stream.str();

      std::transform(opcode.begin()+2, opcode.end(), opcode.begin()+2, ::toupper);

      int cycle = data["unprefixed"][opcode]["cycles"][0];
      
      std::cout << "" << cycle << "," << std::endl;
    }
  }
  std::cout << "};" << std::endl;
  return 0;
}
