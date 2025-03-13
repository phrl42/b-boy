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
        pos = dest.find(n);
    }
}

int main()
{
    std::fstream file("Opcodes.json");

    json data = json::parse(file);


    std::string opcode = "";

    std::cout << "Instruction instructions[256] = \n{" << std::endl;
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

            std::string mnemonic = data["cbprefixed"][opcode]["mnemonic"];

            std::string operand1 = "";
            std::string operand2 = "";

            bool boperand1 = false;
            bool boperand2 = false;

            std::string line = "";

            remove_str(mnemonic, "\"");

            line += "{";
            try
            {
                operand1 = data["cbprefixed"][opcode]["operands"][0]["name"];
                remove_str(operand1, "\"");
                line += "\"" + mnemonic + " " + operand1;

                boperand1 = true;
            }
            catch(const std::exception& e)
            {
                //std::cout << "\"" << mnemonic << "\"," << std::endl;
                //continue;
                line += "\"" + mnemonic;
            }

            try
            {
                operand2 = data["cbprefixed"][opcode]["operands"][1]["name"];
                remove_str(operand2, "\"");
                line += ", " + operand2 + "\", ";

                boperand2 = true;
            }
            catch(const std::exception& e)
            {
                line += "\", ";
            }
            int cycle = data["cbprefixed"][opcode]["cycles"][0];

            bool immediate1 = true;
            bool immediate2 = true;
            try
            {	
                immediate1 = data["cbprefixed"][opcode]["operands"][0]["immediate"];
                immediate2 = data["cbprefixed"][opcode]["operands"][1]["immediate"];
            }
            catch(const std::exception& e)
            {

            }
            line += std::to_string(cycle) + ", ";

	    {
	      line += "&CPU::" + mnemonic + ", ";

	      // first operand

	      // IMode::ALL
	      if(immediate1)
	      {
		if(operand1 == "BC" || operand1 == "DE" || operand1 == "HL" || operand1 == "SP")
		{
		  line += "IMode::ALL, &";
		  line += operand1 + ", ";
		}
	      }
	      // IMode::HIGH
	      if(operand1 == "A") line += "IMode::HIGH, &AF, ";
	      if(operand1 == "B") line += "IMode::HIGH, &BC, ";
	      if(operand1 == "D") line += "IMode::HIGH, &DE, ";
	      if(operand1 == "H") line += "IMode::HIGH, &HL, ";

	      // IMode::LOW
	      if(operand1 == "C") line += "IMode::LOW, &BC, ";
	      if(operand1 == "E") line += "IMode::LOW, &DE, ";
	      if(operand1 == "L") line += "IMode::LOW, &HL, ";

	      if(operand1 == "0") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "1") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "2") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "3") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "4") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "5") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "6") line += "(IMode)" + operand1 + ", nullptr, ";
	      if(operand1 == "7") line += "(IMode)" + operand1 + ", nullptr, ";

	      // IMode::MEM
	      if(!immediate1)
	      {
		line += "IMode::MEM, &" + operand1 + ", ";
	      }

	      // second operand

	      // IMode::ALL
	      if(immediate2)
	      {
		if(operand2 == "BC" || operand2 == "DE" || operand2 == "HL" || operand2 == "SP")
		{
		  line += "IMode::ALL, &";
		  line += operand2;
		}
	      }
	      // IMode::HIGH
	      if(operand2 == "A") line += "IMode::HIGH, &AF";
	      if(operand2 == "B") line += "IMode::HIGH, &BC";
	      if(operand2 == "D") line += "IMode::HIGH, &DE";
	      if(operand2 == "H") line += "IMode::HIGH, &HL";

	      // IMode::LOW
	      if(operand2 == "C") line += "IMode::LOW, &BC";
	      if(operand2 == "E") line += "IMode::LOW, &DE";
	      if(operand2 == "L") line += "IMode::LOW, &HL";

	      if(operand2 == "n8") line += "IMode::N8, nullptr";
	      if(operand2 == "n16") line += "IMode::N16, nullptr";
	      if(operand2 == "e8") line += "IMode::E8, nullptr";

	      if(operand2 == "")
	      {
		line += "IMode::NONE, nullptr";
	      }
	      // IMode::MEM
	      if(!immediate2)
	      {
		line += "IMode::MEM, &" + operand2;
	      }


            }
            if(mnemonic.find("ILLEGAL") != std::string::npos)
            {
	      line += "nullptr, IMode::NONE, nullptr, IMode::NONE, nullptr";
            }
            if(line.size() < 20)
            {
	      line += "CPU::" + mnemonic + ", IMode::NONE, nullptr, IMode::NONE, nullptr";
            }

            line += "}, //" + opcode;

            std::cout << line << std::endl;
        }
    }
    std::cout << "};" << std::endl;
    return 0;
}
