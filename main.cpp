#include "LN.h"
#include "return_codes.h"
#include "test.h"
#include <unordered_map>

#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>

using std::cout, std::to_string, std::string;

std::ostream &operator<<(std::ostream &out, LN value)
{
	if (value.isNan())
	{
		out << "NAN";
		return out;
	}
	if (value.isZero())
	{
		out << '0';
		return out;
	}
	if (value.isNegative())
	{
		out << '-';
	}
	for (int i = value.getSize() - 1; i >= 0; --i)
	{
		out << value.getRef()[i];
	}
	return out;
}

std::string toString(LN value)
{
	if (value.isNan())
	{
		return "NAN";
	}
	if (value.isZero())
	{
		return "0";
	}
	std::string str;
	if (value.isNegative())
	{
		str += '-';
	}
	for (int i = value.getSize() - 1; i >= 0; --i)
	{
		str += '0' + value.getRef()[i];
	}
	return str;
}

const std::unordered_map< char, int > codes{
	{ '0', 0 },	 { '1', 1 },  { '2', 2 },  { '3', 3 },	{ '4', 4 },	 { '5', 5 },  { '6', 6 },  { '7', 7 },
	{ '8', 8 },	 { '9', 9 },  { 'a', 10 }, { 'A', 10 }, { 'b', 11 }, { 'B', 11 }, { 'C', 12 }, { 'C', 12 },
	{ 'd', 13 }, { 'D', 13 }, { 'e', 14 }, { 'E', 14 }, { 'f', 15 }, { 'F', 15 }
};

LN lnPow(int value, int degree)
{
	if (degree == 0)
	{
		return 1;
	}
	LN temp = value;
	for (int i = 1; i < degree; ++i)
	{
		temp *= value;
	}
	return temp;
}

LN toDecimal(std::string str)
{
	LN result;
	bool flag = false;
	if (str[0] == '-')
	{
		flag = true;
		str = str.substr(1, str.size() - 1);
	}
	int size = str.size();
	for (int i = 0; i < size; ++i)
	{
		result += lnPow(16, size - 1 - i) * codes.at(str[i]);
	}
	if (flag)
	{
		result = -result;
	}
	return result;
}

std::string toHex(LN &value)
{
	std::string hex_str;
	hex_str.reserve(value.getSize() + 1);
	if (value.isNan())
	{
		return "NAN";
	}
	if (value.isZero())
	{
		return "0";
	}
	while (!value.isZero())
	{
		LN rem = value % 16;	// mod % 16
		if (rem < 10)
		{
			hex_str = toString(rem) + hex_str;
		}
		else
		{
			hex_str = static_cast< char >('A' + std::stoi(toString(rem - 10))) + hex_str;
		}
		value /= 16;
	}
	if (value.getSign() == -1)
	{
		hex_str = '-' + hex_str;
	}
	return hex_str;
}

bool handle(std::string &line, std::stack< LN > &stack)
{
	if (line.size() == 1 && line[0] == '+')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto result = operand1 + operand2;
		stack.push(result);
		return true;
	}
	if (line.size() == 1 && line[0] == '-')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto result = operand1 - operand2;
		stack.push(result);
		return true;
	}

	if (line.size() == 1 && line[0] == '*')
	{
		auto operand2 = stack.top();
		stack.pop();
		auto operand1 = stack.top();
		stack.pop();

		auto result = operand1 * operand2;
		stack.push(result);
		return true;
	}
	if (line.size() == 1 && line[0] == '/')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto result = operand1 / operand2;
		stack.push(result);
		return true;
	}

	if (line.size() == 1 && line[0] == '%')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto result = operand1 % operand2;
		stack.push(result);
		return true;
	}

	if (line.size() == 1 && line[0] == '~')
	{
		auto operand1 = stack.top();
		stack.pop();

		auto result = ~operand1;
		stack.push(result);
		return true;
	}

	if (line.size() == 1 && line[0] == '_')
	{
		auto operand1 = stack.top();
		stack.pop();

		auto result = -operand1;
		stack.push(result);
		return true;
	}
	if (line.size() == 1 && line[0] == '<')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 < operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	if (line.size() == 1 && line[0] == '>')
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 > operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	if (line.size() == 2 && line == "==")
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 == operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	if (line.size() == 2 && line == "!=")
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 != operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	if (line.size() == 2 && line == "<=")
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 <= operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	if (line.size() == 2 && line == ">=")
	{
		auto operand1 = stack.top();
		stack.pop();
		auto operand2 = stack.top();
		stack.pop();

		auto temp = operand1 >= operand2;
		LN result = temp ? 1 : 0;

		stack.push(result);
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	try
	{
		if (argc != 3)
		{
			throw std::invalid_argument("Incorrect quantity of arguments!");
		}
	} catch (std::invalid_argument &)
	{
		return ERROR_PARAMETER_INVALID;
	}

	std::string input_file_name = argv[1];
	std::string output_file_name = argv[2];

	std::ifstream input_file(input_file_name);
	try
	{
		if (!input_file)
		{
			throw std::runtime_error("Unable to open input file.");
		}
	} catch (std::runtime_error &)
	{
		input_file.close();
		return ERROR_CANNOT_OPEN_FILE;
	}

	std::stack< LN > stack;
	std::string line;

	while (!input_file.eof())
	{
		input_file >> line;
		bool flag = handle(line, stack);
		if (!flag)
		{
			try
			{
				stack.push(toDecimal(line));
			} catch (std::bad_alloc &)
			{
				return ERROR_OUT_OF_MEMORY;
			}
		}
	}
	input_file.close();

	std::ofstream output_file(output_file_name);
	try
	{
		if (!output_file)
		{
			throw std::runtime_error("Unable to open output file.");
		}
	} catch (std::runtime_error &)
	{
		output_file.close();
		return ERROR_CANNOT_OPEN_FILE;
	}

	while (!stack.empty())
	{
		auto val = stack.top();
		stack.pop();
		output_file << toHex(val) << "\n";
	}
	output_file.close();
	return SUCCESS;
}
