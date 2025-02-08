#ifndef DEQUESTRINGSTREAM_H
#define DEQUESTRINGSTREAM_H

#include <deque>
#include <string>

class DequStringStream {
private:
	std::deque<char> buffer;

public:
	void set(const std::string& str) { buffer.assign(str.begin(), str.end()); }
	void front(const std::string& str) { buffer.insert(buffer.end(), str.begin(), str.end()); }
	void back(const std::string& str) { buffer.insert(buffer.begin(), str.begin(), str.end()); }

	std::string str() { return std::string(buffer.begin(), buffer.end()); }
};

#endif // !DEQUESTRINGSTREAM_H
