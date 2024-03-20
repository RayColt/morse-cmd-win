#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <vector>
#include <regex>
#include <windows.h>
#include "Morse-Wav.cpp"

using namespace std;
/**
* C++ Morse Class
*
* @author Ray Colt <ray_colt@pentagon.mil>
* @copyright Copyright (c) 1975, 2021 Ray Colt
* @license Public General License US Army, Microsoft Corporation (MIT)
*
* Derived from ARPANET Pentagon's morse.
*
* You can damage your hearing or your speakers if you play tones at extreme volumes!
* This program will not allow to play morse < 37 Hz and > 8,000 Hz.
*
* Usage program, see: ./morse -help or -h
*
* The Math: sine wave: y(t) = amplitude * sin(2 * PI * frequency * time), time = s / sample_rate
**/

class Morse
{
private:
	const string error_in = "INPUT-ERROR";

public:
	double frequency_in_hertz = 880.0;// 880 Hz music note A5 - 440 cycles every second
	double words_per_minute = 16.0;//words per minute
	double max_frequency_in_hertz = 8000.0;
	double min_frequency_in_hertz = 37.0;
	double samples_per_second = 44100.0;
	/**
	* Constructor
	*/
public:
	Morse() { fill_morse_maps(); }

private:
	/**
	* Fill Multimaps with morse tables
	*/
	multimap <string, string> morse_map;
	multimap <string, string> morse_map_reversed;
	void fill_morse_maps()
	{
		morse_map.insert(pair <string, string>(" ", ""));        // SPACE (0b1)
		morse_map.insert(pair <string, string>("!", "101011"));  // -.-.--
		morse_map.insert(pair <string, string>("$", "0001001")); // ...-..-
		morse_map.insert(pair <string, string>("\"", "010010")); // .-..-.

		morse_map.insert(pair <string, string>("'", "011110"));  // .----.
		morse_map.insert(pair <string, string>("(", "10110"));   // -.--.
		morse_map.insert(pair <string, string>(")", "101101"));  // -.--.-

		morse_map.insert(pair <string, string>(",", "110011"));  // --..--
		morse_map.insert(pair <string, string>("-", "100001"));  // -....-
		morse_map.insert(pair <string, string>(".", "010101"));  // .-.-.-
		morse_map.insert(pair <string, string>("/", "10010"));   // -..-.

		morse_map.insert(pair <string, string>("0", "11111"));   // -----
		morse_map.insert(pair <string, string>("1", "01111"));   // .----
		morse_map.insert(pair <string, string>("2", "00111"));   // ..---
		morse_map.insert(pair <string, string>("3", "00011"));   // ...--
		morse_map.insert(pair <string, string>("4", "00001"));   // ....-
		morse_map.insert(pair <string, string>("5", "00000"));   // .....
		morse_map.insert(pair <string, string>("6", "10000"));   // -....
		morse_map.insert(pair <string, string>("7", "11000"));   // --...
		morse_map.insert(pair <string, string>("8", "11100"));   // ---..
		morse_map.insert(pair <string, string>("9", "11110"));   // ----.

		morse_map.insert(pair <string, string>(":", "111000"));  // ---...
		morse_map.insert(pair <string, string>(";", "101010"));  // -.-.-.
		morse_map.insert(pair <string, string>("=", "10001"));   // -...-
		morse_map.insert(pair <string, string>("?", "001100"));  // ..--..
		morse_map.insert(pair <string, string>("@", "011010"));  // .--.-.

		morse_map.insert(pair <string, string>("A", "01"));      // .-
		morse_map.insert(pair <string, string>("B", "1000"));    // -...
		morse_map.insert(pair <string, string>("C", "1010"));    // -.-.
		morse_map.insert(pair <string, string>("D", "100"));     // -..
		morse_map.insert(pair <string, string>("E", "0"));       // .
		morse_map.insert(pair <string, string>("F", "0010"));    // ..-.
		morse_map.insert(pair <string, string>("G", "110"));     // --.
		morse_map.insert(pair <string, string>("H", "0000"));    // ....
		morse_map.insert(pair <string, string>("I", "00"));      // ..
		morse_map.insert(pair <string, string>("J", "0111"));    // .---
		morse_map.insert(pair <string, string>("K", "101"));     // -.-
		morse_map.insert(pair <string, string>("L", "0100"));    // .-..
		morse_map.insert(pair <string, string>("M", "11"));      // --
		morse_map.insert(pair <string, string>("N", "10"));      // -.
		morse_map.insert(pair <string, string>("O", "111"));     // ---
		morse_map.insert(pair <string, string>("P", "0110"));    // .--.
		morse_map.insert(pair <string, string>("Q", "1101"));    // --.-
		morse_map.insert(pair <string, string>("R", "010"));     // .-.
		morse_map.insert(pair <string, string>("S", "000"));     // ...
		morse_map.insert(pair <string, string>("T", "1"));       // -
		morse_map.insert(pair <string, string>("U", "001"));     // ..-
		morse_map.insert(pair <string, string>("V", "0001"));    // ...-
		morse_map.insert(pair <string, string>("W", "011"));     // .--
		morse_map.insert(pair <string, string>("X", "1001"));    // -..-
		morse_map.insert(pair <string, string>("Y", "1011"));    // -.--
		morse_map.insert(pair <string, string>("Z", "1100"));    // --..
		morse_map.insert(pair <string, string>("_", "001101"));  // ..--.-
		morse_map.insert(pair <string, string>("ERR", "00000000")); // ........
		// invert morse_map
		for (const auto& it : morse_map)
		{
			morse_map_reversed.insert(make_pair(it.second, it.first));
		}
	}

private:
	/**
	* Get binary morse code (dit/dah) for a given character
	*
	* @param character
	* @return string
	*/
	string getBinChar(string character)
	{
		return morse_map.find(character)->second;
	}

private:
	/**
	* Get morse code (dit/dah) for a given character
	*
	* @param character
	* @return string
	*/
	string getMorse(string character)
	{
		return strtr(morse_map.find(character)->second, "01", ".-");
	}

private:
	/**
	* Get character for given morse code
	*
	* @param morse
	* @return string
	*/
	string getCharacter(string morse)
	{
		return morse_map_reversed.find(strtr(morse, ".-", "01"))->second;
	}

public:
	/**
	* Get binary morse code for given string
	*
	* @param str
	* @return string
	*/
	string morse_binary(string str)
	{
		string line = "";
		str = fix_input(str);
		str = regex_replace(str, regex("\\s{2,}"), " ");
		str = regex_replace(str, regex("[\t]+"), " ");
		for (size_t i = 0; i < str.length(); i++)
		{
			string chr = str.substr(i, 1);
			line += getBinChar(stringToUpper(chr));
			line += " ";
		}
		return trim(line);
	}

public:
	/**
	* Get morse code for given string
	*
	* @param str
	* @return string
	*/
	string morse_encode(string str)
	{
		string line = "";
		str = fix_input(str);
		str = regex_replace(str, regex("\\s{2,}"), " ");
		str = regex_replace(str, regex("[\t]+"), " ");
		for (size_t i = 0; i < str.length(); i++)
		{
			string chr = str.substr(i, 1);
			line += getMorse(stringToUpper(chr));
			line += " ";
		}
		return trim(line);
	}

public:
	/**
	* Get character string for given morse code
	*
	* @param str
	* @return string
	*/
	string morse_decode(string str)
	{
		string line = "";
		str = regex_replace(str, regex("[\t]+"), " ");
		if (regex_match(str, regex("[10\\s\\.\\-]+")))
		{
			vector<string> morsecodes = explode(str, ' ');
			for (auto morse : morsecodes)
			{
				if (morse.empty())
					line += " ";
				if (morse.size() < 9)
				{
					line += getCharacter(morse);
				}
			}
			return regex_replace(line, regex("\\s{2,}"), " ");
		}
		else
		{
			return error_in;
		}
	}

public:
	/**
	* (\  /)
	* ( .  .)
	* Get hexadecimal morse code for given string
	*
	* @param str
	* @param modus
	* @return string
	*/
	string bin_morse_hexadecimal(string str, int modus)
	{
		string str1, str2;
		const char* a[] = { "2E ", "2D ", "30 ", "31 " };
		if (modus == 0) { str1 = a[0]; str2 = a[1]; };
		if (modus == 1) { str1 = a[2]; str2 = a[3]; };
		string line = morse_binary(str);
		line = regex_replace(line, regex("  "), "A");
		line = regex_replace(line, regex(" "), "K");
		line = regex_replace(line, regex("0"), str1);
		line = regex_replace(line, regex("1"), str2);
		line = regex_replace(line, regex("A"), "20 20 ");
		line = regex_replace(line, regex("K"), "20 ");
		return trim(line);
	}

public:
	/**
	* (\  /)
	* ( .  .)
	* Get txt for given hexadecimal morse code
	*
	* @param str
	* @param modus
	* @return string
	*/
	string hexadecimal_bin_txt(string str, int modus)
	{
		string str1, str2;
		if (regex_match(str, regex("[20|30|31|2D|2E|\\s]+")))
		{
			const char* a[] = { "2E", "2D", "30", "31" };
			if (modus == 0) { str1 = a[0]; str2 = a[1]; };
			if (modus == 1) { str1 = a[2]; str2 = a[3]; };
			string line = remove_whitespaces(str);
			line = regex_replace(line, regex("2020"), "  ");
			line = regex_replace(line, regex("20"), " ");
			line = regex_replace(line, regex(str1), "0");
			line = regex_replace(line, regex(str2), "1");
			string s = morse_decode(trim(line));
			return s;
		}
		else
		{
			return error_in;
		}
	}

private:
	/**
	* A function that converts a string to uppercase letters
	*
	* @param str
	* @return string
	*/
	string stringToUpper(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

private:
	/**
	* Similar to strtr in php, characters in 'from' will be <br>
	* replaced by characters in 'to' in the same <br>
	* order character by character.
	*
	* @param str
	* @param from
	* @param to
	* @return string
	*/
	string strtr(string str, string from, string to)
	{
		vector<string> out;
		for (size_t i = 0, len = str.length(); i < len; i++)
		{
			char c = str.at(i);
			int p = from.find(c);
			if (p >= 0)
			{
				char t = to.at(p);
				out.push_back(string(1, t));
			}
		}
		return !out.empty() ? stringArrToString(out) : str;
	}

private:
	/**
	* Convert a string array into a string <br>
	*
	* @param vstr
	* @return string
	*/
	string stringArrToString(vector<string> vstr)
	{
		string scr = "";
		if (!vstr.empty())
		{
			for (size_t i = 0; i < vstr.size(); i++)
			{
				scr += vstr[i];
			}
		}
		return scr;
	}

public:
	/**
	* trimp automatically strips space at the start and end of a given string <br>
	*
	* @param str
	* @return string
	*/
	string trim(const string& str)
	{
		size_t first = str.find_first_not_of(' ');
		if (string::npos == first)
		{
			return str;
		}
		size_t last = str.find_last_not_of(' ');
		return str.substr(first, (last - first + 1));
	}

private:
	/**
	* Similar to explode in php, a split function on a delimiter <br>
	*
	* @param s
	* @param c
	* @return vector
	*/
	const vector<string> explode(const string& s, const char& c)
	{
		string buff;
		vector<string> vstr;
		for (auto str : s)
		{
			if (str != c)
			{
				buff += str;
			}
			else
			{
				vstr.push_back(buff);
				buff = "";
			}
		}
		if (buff != "")
			vstr.push_back(buff);
		return vstr;
	}

private:
	/**
	* Fix input with whitespace to reduce errors
	* info: regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s])" };
	*
	* @param str
	* @return string
	*/
	string fix_input(string str)
	{
		string ret = "";
		regex e("[^a-zA-Z0-9!'\"@/_=\\s\\$\\(\\)\\,\\.\\:\\;\\?\\-]+");
		sregex_token_iterator iter(str.begin(), str.end(), e, -1), end;
		vector<string> vec(iter, end);
		for (auto a : vec)
		{
			ret += a + " ";
		}
		return trim(ret);
	}

private:
	/**
	* Remove all whitespaces for given string
	*
	* @param str
	* @return string
	*/
	string remove_whitespaces(string str)
	{
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		return str;
	}

public:
	/**
	* Calculate words per second to the duration in milliseconds
	*
	* Dit: 1 unit
	* Dah: 3 units
	* Intra-character space: 1 unit
	* Inter-character space: 3 Farnsworth-units
	* Word space: longer than 7 Farnsworth-units
	* Standard word Paris is 50 units, elements. (with one extra word space)
	*
	* @param wpm - words per minute
	* @return double
	*/
	double duration_milliseconds(double wpm)
	{
		double ms = 0.0;
		if (!wpm <= 0.0)
		{
			//50 elements per word
			double wps = wpm / 60.0; //words per second
			double eps = 50 * wps; //elements per second
			ms = 1000.0 / eps; //milliseconds per element
			return ms;
		}
		else return ms;
	}

public:
	/**
	* Reaf cmd line user arguments
	*
	* @param argc
	* @param argv[]
	* @return int
	*/
	int get_options(int argc, char* argv[])
	{
		int args = 0;
		bool ok = false;
		if (strncmp(argv[1], "e", 1) == 0 || strncmp(argv[1], "b", 1) == 0 || strncmp(argv[1], "d", 1) == 0 ||
			strncmp(argv[1], "he", 2) == 0 || strncmp(argv[1], "hd", 2) == 0 || strncmp(argv[1], "hb", 2) == 0 ||
			strncmp(argv[1], "hbd", 3) == 0)
		{
			ok = true;
		}
		if (strncmp(argv[1], "-help", 5) == 0 || strncmp(argv[1], "-h", 2) == 0)
		{
			cout << "#######################################################################\n";
			cout << "## MORSE HELP                                             PLEH ESROM ##\n";
			cout << "#######################################################################\n";
			cout << "Morse Dictionary, chars(url save) and spaces, lower case will be made upper case:\n";
			cout << "ABC DEFGHIJKLMNOPQRSTUVWXYZ 12 34567 890 !$ ' \" (), . _ - / : ; = ? @ \n\n";
			cout << "Usage console app version:\n morse.exe or morse\n\n";
			cout << "Usage cmd line version:\n morse.exe [modus] 'morse or txt'\n\n";
			cout << "Select Modus for encoding or decoding:\n";
			cout << "e, d    : [. - space] Morse Normal\n";
			cout << "b, d    : [0 1 space] Binary Morse\n";
			cout << "he, hd  : [2E 2D 20] HeX Morse\n";
			cout << "hb, hbd : [30 31 20] HeX Binary\n\n";
			cout << "Select modus for encoding to sound :\n";
			cout << "ew  : [Morse to Wav] Windows Wav Stereo - with local sound file\n";
			cout << "ewm : [Morse to Wav] Windows Wav Mono - with local sound file\n";
			cout << "es  : [Morse to Windows beep] Windows Speaker Beep - no sps\n\n";
			cout << "Example: ./morse.exe d \"... ---  ...  ---\"\n";
			cout << "(only with decoding, option d, double quotes are necessary to preserve double spaces who create words)\n\n";
			cout << "Sound settings:\n";
			cout << "Tone(Hz), tone frequency in Herz, allowed between 20 Hz - 8000 Hz\n";
			cout << "WPM, words per minute, allowed between 0 wpm - 50 wpm\n";
			cout << "SPS, samples per second, allowed between 8000 Hz - 48000 Hz\n";
			cout << "For creating sound files there is a maximum of 750 chars, bigger text might lead to a long term 'not responding'.\n\n";
			cout << "For inspiration have look at music notes their frequencies.\n";
			cout << "Example: ./morse.exe es -wpm:18 -hz:739.99 paris paris paris (sps not available in es mode)\n";
			cout << "Example: ./morse.exe ew paris paris paris\n";
			cout << "Example: ./morse.exe ew -wpm:16 -hz:880 paris paris paris\n";
			cout << "Example: ./morse.exe ewm -wpm:16 -hz:880 -sps:44100 paris paris paris\n";
			cout << "Example: ./morse.exe ew -wpm:20 -hz:1050 -sps:22050 paris paris paris\n\n";
			cout << "#######################################################################";
			ok = true;
		}
		else if (ok)
		{
			while (argc > 1)
			{
				if (strncmp(argv[2], "-hz:", 4) == 0)
				{
					frequency_in_hertz = atof(&argv[2][4]);
					if (frequency_in_hertz > max_frequency_in_hertz) frequency_in_hertz = max_frequency_in_hertz;
					if (frequency_in_hertz < min_frequency_in_hertz) frequency_in_hertz = min_frequency_in_hertz;
				}
				else if (strncmp(argv[2], "-wpm:", 5) == 0)
				{
					words_per_minute = atof(&argv[2][5]);
				}
				else if (strncmp(argv[2], "-sps:", 5) == 0)
				{
					samples_per_second = atof(&argv[2][5]);
				}
				else
				{
					break;
				}
				argc -= 1;
				argv += 1;
				args += 1;
			}
		}
		else
		{
			fprintf(stderr, "option error %s, see morse -help for info\n", argv[2]);
			exit(1);
		}
		return args;
	}

public:
	/**
	* Generate string from arguments
	* @param arg
	*
	* @return string
	*/
	string arg_string(char* arg)
	{
		char c; string str;
		while ((c = *arg++) != '\0')
		{
			str += c;
		}
		str += " ";
		return str;
	}
};

/**
* Main Class
*/
int main(int argc, char* argv[])
{
	Morse m;
	int n;
	string action = "encode";
	double sps = 44100;
	if (argc != 1)
	{
		if (strcmp(argv[1], "es") == 0) action = "sound"; else
			if (strcmp(argv[1], "ew") == 0) action = "wav"; else
				if (strcmp(argv[1], "ewm") == 0) action = "wav_mono"; else
					if (strcmp(argv[1], "e") == 0) action = "encode"; else
						if (strcmp(argv[1], "d") == 0) action = "decode"; else
							if (strcmp(argv[1], "b") == 0) action = "binary"; else
								if (strcmp(argv[1], "he") == 0) action = "hexa"; else
									if (strcmp(argv[1], "hd") == 0) action = "hexadec"; else
										if (strcmp(argv[1], "hb") == 0) action = "hexabin"; else
											if (strcmp(argv[1], "hbd") == 0) action = "hexabindec";
		// check options
		n = m.get_options(argc, argv);
		argc -= n;
		argv += n;
		// generate morse code
		string str;
		while (argc > 2)
		{
			str += m.arg_string(argv[2]);
			argc -= 1;
			argv += 1;
		}
		if (action == "encode") cout << m.morse_encode(str) << "\n"; else
			if (action == "binary") cout << m.morse_binary(str) << "\n"; else
				if (action == "decode") cout << m.morse_decode(str) << "\n"; else
					if (action == "hexa") cout << m.bin_morse_hexadecimal(str, 0) << "\n"; else
						if (action == "hexadec") cout << m.hexadecimal_bin_txt(str, 0) << "\n"; else
							if (action == "hexabin") cout << m.bin_morse_hexadecimal(str, 1) << "\n"; else
								if (action == "hexabindec") cout << m.hexadecimal_bin_txt(str, 1) << "\n"; else
									if (action == "sound" || action == "wav" || action == "wav_mono")
									{
										cout << "-wpm: " << m.words_per_minute << " (" << m.duration_milliseconds(m.words_per_minute) << " ms)\n";
										cout << "-hz: " << m.frequency_in_hertz << "Hz (tone)\n";
										str.resize(750);
										string morse = m.morse_encode(str);
										cout << morse << "\n";
										if (action == "wav")
										{
											MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
										}
										else if (action == "wav_mono")
										{
											MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
										}
										else
										{
											int size = morse.size();
											printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
											printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
											printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
											cout << "to change Tone(Hz) and WPM use cmd morse.exe -help or -h for info\n";
											for (size_t i = 0; i < size; ++i)
											{
												char c = morse.at(i);
												string s(1, c);
												if (s == ".") Beep(m.frequency_in_hertz, 1 * m.duration_milliseconds(m.words_per_minute));
												if (s == "-") Beep(m.frequency_in_hertz, 3 * m.duration_milliseconds(m.words_per_minute));
												if (s == " ") Beep(0, 3.5 * m.duration_milliseconds(m.words_per_minute));
											}
										}
									}
	}
	else
	{
		string arg_in;
		cout << "MORSE (CMD line version: morse.exe -help or -h for info)\n";
		cout << "morse table: \nABC DEFGHIJKLMNOPQRSTUVWXYZ 12 34567 890 ! $ ' \" (), . _ - / : ; = ? @ \n";
		cout << "morse actions: \n0 [encode with sound], 1 [encode with stereo sound to wav file] \n2 [encode with mono sound to wav file]\n";
		cout << "3 [encode], 4 [binary encode], 5 [decode morse/binary].\n";
		cout << "6 [hexa encode], 7 [hexa decode].\n";
		cout << "8 [hexa bin encode], 9 [hexa bin decode].\n";
		cout << "choose action 0,1,2,3,4,5,6,7,8 or 9 and press [enter]:\n";
		getline(cin, arg_in);
		regex e("[0-9]");
		if (!regex_match(arg_in, e))
		{
			arg_in = "3";
			cout << "wrong input, action " << arg_in << " is active now" << "\n";
		}
		if (regex_match(arg_in, e))
		{
			if (arg_in == "0") action = "sound"; else
				if (arg_in == "1") action = "wav"; else
					if (arg_in == "2") action = "wav_mono"; else
						if (arg_in == "3") action = "encode"; else
							if (arg_in == "4") action = "binary"; else
								if (arg_in == "5") action = "decode"; else
									if (arg_in == "6") action = "hexa"; else
										if (arg_in == "7") action = "hexadec"; else
											if (arg_in == "8") action = "hexabin"; else
												if (arg_in == "9") action = "hexabindec";

			cout << "type or paste input and press [enter]\n";
			getline(cin, arg_in);
			if (action == "sound" || action == "wav" || action == "wav_mono")
			{
				arg_in.resize(750);
				string str = m.morse_encode(arg_in);
				cout << str << "\n";
				if (action == "wav")
				{
					MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
				}
				else if (action == "wav_mono")
				{
					MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
				}
				else
				{
					int size = str.size();
					printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
					printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
					printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
					cout << "to change Tone(Hz) and WPM use cmd morse.exe -help or -h for info\n";
					for (size_t i = 0; i < size; ++i)
					{
						char c = str.at(i);
						string s(1, c);
						if (s == ".") Beep(m.frequency_in_hertz, 1 * m.duration_milliseconds(m.words_per_minute));
						if (s == "-") Beep(m.frequency_in_hertz, 3 * m.duration_milliseconds(m.words_per_minute));
						if (s == " ") Beep(0, 3.5 * m.duration_milliseconds(m.words_per_minute));
					}
				}
			}
			else
				if (action == "encode") cout << m.morse_encode(arg_in) << "\n"; else
					if (action == "binary") cout << m.morse_binary(arg_in) << "\n"; else
						if (action == "decode") cout << m.morse_decode(arg_in) << "\n"; else
							if (action == "hexa") cout << m.bin_morse_hexadecimal(arg_in, 0) << "\n"; else
								if (action == "hexadec") cout << m.hexadecimal_bin_txt(arg_in, 0) << "\n"; else
									if (action == "hexabin") cout << m.bin_morse_hexadecimal(arg_in, 1) << "\n"; else
										if (action == "hexabindec") cout << m.hexadecimal_bin_txt(arg_in, 1) << "\n";
		}
		cout << "Press any key to close program . . .";
		int c = getchar();
		return 0;
	}
}