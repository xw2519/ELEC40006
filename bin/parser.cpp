#include "parser.hpp"

std::vector<std::string> tokeniser (std::string input)
{
    // Remove '(' char if present
    replace(input.begin(), input.end(), '(', ' ');

    // Remove ')' char if present
    input.erase(std::remove(input.begin(), input.end(), ')'), input.end());

    std::istringstream iss(input);
    std::vector<std::string> tokensied ((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    return tokensied;
}

bool is_digit(const std::string& input)
{
    std::string::const_iterator it = input.begin();
    while (it != input.end() && std::isdigit(*it) || it != input.end() && (*it=='.')) ++it;
    return !input.empty() && it == input.end();
}

float converter(const std::string& val_str)
{
    float digits;
    // Check if there are any abbreviations
    size_t abbre_pos = val_str.find_first_of("fpnumkMGT");

    if (abbre_pos==std::string::npos)
    {
        // No recognised abbreviation, extract digits
        std::string str_digits;
        for (char const &c: val_str)
        {
            if (isdigit(c))
            {
                str_digits = str_digits + c;
            }
        }
        
        if (str_digits.size() == 0)
        {
            return 0;
        }
        else
        {
            return std::stof(str_digits);
        }
    }

    // A recognised abbreviation detected
    std::string str_digits = val_str.substr(0, (abbre_pos));

    digits = stof(str_digits);
    switch (val_str[abbre_pos])
    {
    case 'f':
        digits = digits/1000000000000000;
        break;
    case 'p':
        digits = digits/1000000000000;
        break;
    case 'n':
        digits = digits/1000000000;
        break;
    case 'u':
        digits = digits/1000000;
        break;
    case 'm':
        digits = digits/1000;
        break;
    case 'k':
        digits = digits*1000;
        break;
    case 'M':
        digits = digits*1000000;
        break;
    case 'G':
        digits = digits*1000000000;
        break;
    case 'T':
        digits = digits*1000000000000;
        break;
    default: // Ignore it
        break;
    }

    return digits;
}

void parser(std::istream& cin, std::vector<CirElement>& circuit, std::vector<CirSrc>& sources, std::vector<CirFunctions>& functions)
{
    CirElement x;
    CirSrc y;
    CirFunctions z;
    std::vector<std::string> store;
    std::string line;

    // First line is always the TITLE which is skipped.
    bool firsttime = true;

    while (std::getline(std::cin, line))
    {
        // Check for beginning TITLE
        if (firsttime)
        {
            firsttime = false;
            continue;
        }

        // Check for .end
        if (line == ".end")
        {
            break;
        }

        // Check if this line is a comment, denoted by '*'
        std::size_t presence = line.find('*');
        if (presence!=std::string::npos)
        {
            /*
            std::cout << "Comment detected." << std::endl;
            */
            continue;
        }
        
        //Tokenise
        store = tokeniser(line);

        // Get circuit element name
        // If R, C, L then proceed normal
        if (tolower(store[0][0]) == 'r' || tolower(store[0][0]) == 'c' || tolower(store[0][0]) == 'l')
        {
            // Store the element and description
            x.D = store[0][0];
            store[0].erase(store[0].begin() + 0);
            x.descrip = store[0];

            // Get nodes in the format "n1", "n2", etc
            // Check if node is grounded represented as '0'
            if (tolower(store[1][0]) != 'n' && tolower(store[2][0]) == 'n')
            {
                x.n1 = stoi(store[1]);
                x.n2 = stoi(store[2].erase(0,1));
            }
            else if (tolower(store[1][0]) == 'n' && tolower(store[2][0]) != 'n')
            {
                x.n1 = stoi(store[1].erase(0,1));
                x.n2 = stoi(store[2]);
            }
            else if (tolower(store[1][0]) != 'n' && tolower(store[2][0]) != 'n')
            {
                std::cerr << "Incorrect nodes inputted." << std::endl;
            }
            else
            {
                // Remove 'N' char from string
                x.n1 = stoi(store[1].erase(0,1));
                x.n2 = stoi(store[2].erase(0,1));
            }
            
            // Detect and get values. Must have values
            if (store.size() < 3)
            {
                std::cerr << "No values entered." << std::endl;
            }
            std::string values;
            values = store[3];

            // Ensure all digits
            //assert(is_digit(values));
            x.value = converter(values);

            // Push into vector representing circuit inputted
            circuit.push_back(x);
        }

        // If V or I then store in 'source' vector
        else if (tolower(store[0][0]) == 'v' || tolower(store[0][0]) == 'i')
        {
            // Store the element and description
            y.D = store[0][0];
            store[0].erase(store[0].begin() + 0);
            y.descrip = store[0];

            // Get nodes in the format "n1", "n2", etc
            // Check if node is grounded represented as '0'
            if (tolower(store[1][0]) != 'n' && tolower(store[2][0]) == 'n')
            {
                y.n1 = stoi(store[1]);
                y.n2 = stoi(store[2].erase(0,1));
            }
            else if (tolower(store[1][0]) == 'n' && tolower(store[2][0]) != 'n')
            {
                y.n1 = stoi(store[1].erase(0,1));
                y.n2 = stoi(store[2]);
            }
            else if (tolower(store[1][0]) != 'n' && tolower(store[2][0]) != 'n')
            {
                std::cerr << "Nodes are inputted wrongly" << std::endl;
            }
            else
            {
                // Remove 'N' char from string
                y.n1 = stoi(store[1].erase(0,1));
                y.n2 = stoi(store[2].erase(0,1));
            }

            if (store[3] != "SINE") // Case of DC input
            {
                // Detect and get values. Must have values
                if (store.size() < 3)
                {
                    std::cerr << "No values entered." << std::endl;
                    exit;
                }

                // Ensure all digits
                //assert(is_digit(values));
                y.type = "DC";
                y.DC = converter(store[3]);
                y.A = 0;
                y.freq = 0;

                // Push into sources vector
                sources.push_back(y);
            }

            else if (store[3] == "SINE")
            {
                // Ensure all digits
                //assert(is_digit(values));
                y.type = "SINE";
                y.DC = converter(store[4]);
                y.A = converter(store[5]);
                y.freq = converter(store[6]);

                // Push into sources vector
                sources.push_back(y);
            }

            else
            {
                std::cerr << "Source type unrecognised." << std::endl;
                exit;
            }
        }

        // Recognise .tran function
        else if (store[0] == ".tran")
        {
            // Store function parameter
            z.func_name = store[0];
            z.tprint = stoi(store[1]);
            z.tstop = stoi(store[2]);
            z.tstart = stoi(store[3]);
            z.tmax = stoi(store[4]);

            // Push into vector
            functions.push_back(z);
        }

        else
        {
            //std::cout << buf[0] << std::endl;
            std::cerr << "Unknown netlist line entered" << std::endl;
            exit;
        }

        store.clear();
    }
}

int N_int(const std::vector<CirElement> &circuit)
{
    int largest = 0;

    // Scan vector for largest number of nodes
    for (int i = 0; i < circuit.size(); i++)
    {   
        if (circuit[i].n1 > largest)
        {
            largest = circuit[i].n1;
        }
        else if (circuit[i].n2 > largest)
        {
            largest = circuit[i].n2;
        }
    }
    return largest;
}

int M_int(const std::vector<CirSrc> &voltages)
{
    return voltages.size();
}