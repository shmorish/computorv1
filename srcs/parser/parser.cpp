#include "parser.hpp"

static bool isTokenVar(const std::string &token, std::string *var)
{
  if (*var != "")
    throw std::invalid_argument("Multiple variables");
  if (token == "X")
  {
    *var = "X^1";
    return true;
  }
  else if (token.size() > 2 && token[0] == 'X' && token[1] == '^')
  {
    *var = token;
    return true;
  }
  return false;
}
// Store the tokens
void Parser::store_tokens(std::list<std::list<std::string>> &tokens)
{
  for (std::list<std::string> token : tokens)
  {
    std::string var = "";
    double number = 1;
    for (std::string t : token)
    {
      if (isTokenVar(t, &var))
        continue;
      else
      {
        number = number * std::stod(t);
      }
    }
    if (var == "")
      _formula[0] += number;
    else
    {
      _formula[std::stoi(var.substr(2))] += number;
    }
  }
}

void Parser::printReducedForm()
{
  bool first = true;
  std::cout << "Reduced form: ";
  for (auto it = _formula.begin(); it != _formula.end(); it++)
  {
    if (it->second != 0)
    {
      if (it->second > 0 && !first)
        std::cout << "+ ";
      if (it->second < 0)
        std::cout << "- ";
      std::cout << fabs(it->second) << " " << "* X^" << it->first << " ";
      first = false;
    }
  }
  std::cout << "= 0" << std::endl;
}

void Parser::printPolynomialDegree()
{
  for (auto it = _formula.begin(); it != _formula.end(); it++)
  {
    if (it->second != 0)
      _degree = it->first;
  }
  std::cout << "Polynomial degree: " << _degree << std::endl;
  if (_degree > 2)
    throw std::invalid_argument("The polynomial degree is stricly greater than 2, I can't solve.");
}

Parser::Parser(std::string &line) : _formula({{0, 0}, {1, 0}, {2, 0}})
{
  try
  {
    std::list<std::list<std::string>> tokens = split_equation_into_tokens(line);
    store_tokens(tokens);
    printReducedForm();
    printPolynomialDegree();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    std::exit(1);
  }
}
Parser::~Parser()
{
}

// Split the equation into tokens
// 1. Split the equation into tokens
// 2. Return the list of tokens
std::list<std::list<std::string>> Parser::split_equation_into_tokens(const std::string &line)
{
  std::istringstream iss(line);
  std::list<std::string> tokens;
  std::string token;
  while (iss >> token)
  {
    tokens.push_back(token);
  }
  check_tokens(tokens);

  std::list<std::list<std::string>> eqation_tokens;
  std::list<std::string> eqation_token;
  bool is_equal = false;
  while (!tokens.empty())
  {
    std::string token = tokens.front();
    tokens.pop_front();
    if (token != "+" && token != "-" && token != "=" && token != "*")
    {
      eqation_token.push_back(token);
      continue;
    }
    else if (token == "+" || token == "-")
    {
      eqation_tokens.push_back(eqation_token);
      eqation_token.clear();
      if (token == "-" || is_equal)
      {
        eqation_token.push_back("-1");
      }
      continue;
    }
    else if (token == "=")
    {
      if (is_equal)
        throw std::invalid_argument("Invalid equation");
      is_equal = true;
      eqation_tokens.push_back(eqation_token);
      eqation_token.clear();
      eqation_token.push_back("-1");
      continue;
    }
  }
  eqation_tokens.push_back(eqation_token);
  return eqation_tokens;
}

static bool isNumberOrVariable(const std::string &token)
{
  if (token == "X")
    return true;
  // if the token starts with X^, it is a variable
  if (token.size() > 2 && token[0] == 'X' && token[1] == '^')
  {
    char *end;
    std::strtol(token.substr(2).c_str(), &end, 10);
    if (*end == '\0')
      return true;
  }
  char *end;
  std::strtod(token.c_str(), &end);
  if (*end == '\0')
    return true;
  return false;
}

static bool isOperator(const std::string &token)
{
  if (token == "+" || token == "-" || token == "=" || token == "*")
    return true;
  return false;
}

// Check the tokens
// 1. Check the number of tokens
// 2. Check the format of the tokens
// 3. Set the values of the tokens
void Parser::check_tokens(std::list<std::string> &tokens)
{
  // "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
  // check 文字or数字, 演算子が交互になっているか
  bool prev_is_number_or_variable = false;
  bool have_equal = false;
  for (std::string token : tokens)
  {
    if (token == "=")
    {
      if (have_equal)
        throw std::invalid_argument("Have multiple equal signs");
      have_equal = true;
    }
    if (isNumberOrVariable(token))
    {
      if (prev_is_number_or_variable)
        throw std::invalid_argument("Invalid token: " + token);
      prev_is_number_or_variable = true;
    }
    else if (isOperator(token))
    {
      if (!prev_is_number_or_variable)
        throw std::invalid_argument("Invalid token: " + token);
      prev_is_number_or_variable = false;
    }
    else
    {
      throw std::invalid_argument("Invalid token: " + token);
    }
  }
  if (!prev_is_number_or_variable)
    throw std::invalid_argument("Invalid equation");
  if (!have_equal)
    throw std::invalid_argument("No equal sign");
}
