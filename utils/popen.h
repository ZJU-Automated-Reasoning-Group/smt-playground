#pragma once

#include <unistd.h>
#include <signal.h>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>

#define BUF_SIZE 256

class Popen {
	pid_t pid;
	int infp;
	int outfp;
	std::ostringstream debugInfo;
public:
	Popen(const char *file, char *const args[]) {
		pid = popen2(&infp, &outfp, file, args);
	}

	Popen(const Popen &other) :
			pid(other.pid), infp(other.infp), outfp(other.outfp), debugInfo(
					other.debugInfo.str()) {
	}

	~Popen() {
		close(infp);
		close(outfp);
		kill(pid, 9);
	}

	friend Popen& operator<<(Popen &out, std::string str) {
		out.debugInfo << str;
		write(out.infp, str.c_str(), str.size());
		return out;
	}

	friend std::ostream& operator<<(std::ostream &out, const Popen &popen) {
		char buf[BUF_SIZE];
		memset(buf, '\0', sizeof(buf));
		read(popen.outfp, buf, sizeof(buf));
		out << buf;
		return out;
	}

	std::string getOutput();

	std::string getDebugInfo() const {
		return debugInfo.str();
	}

private:
	bool isEnd(std::string str) const;

	static pid_t popen2(int *intfp, int *outfp, const char *file,
			char *const args[]);
};

