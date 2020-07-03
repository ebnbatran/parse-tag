#include <fstream>
#include <cstdlib>
#include <parsetag/id3.hpp>

namespace ParseTag {

    ID3::ID3() {
        // do nothing
    }

    std::string ID3::parseFrame(const std::string& frameID, const std::string& data) {
        std::string content;

        auto frameIDBegin = data.find(frameID);
        if (frameIDBegin == std::string::npos) {
            return "NOT FOUND";
        }
        auto frameIDEnd = frameIDBegin + 3;
        auto frameSize = (unsigned int)data[frameIDEnd + 1] * 2097152 +
                         (unsigned int)data[frameIDEnd + 2] * 16384 +
                         (unsigned int)data[frameIDEnd + 3] * 128 +
                         (unsigned int)data[frameIDEnd + 4];
        auto frameSizeEnd = frameIDEnd + 4;
        auto frameFlagEnd = frameSizeEnd + 2;
        
        for (int i = frameFlagEnd + 1; i < (frameFlagEnd + frameSize); i++) {
            if (
                (data[i] < ' ') || 
                (data[i] > '~')
            ) {
                continue;
            }
            content.push_back(data[i]);
        }

        return content;
    }

    bool ID3::parseFromFile(const std::string& path) {
        // Open the file and read its contents
        std::ifstream file{path};
        if (!file.is_open()) {
            return false;
        }
        file.seekg(0, std::ios::end);
        std::streamsize fsize = file.tellg();
        if (fsize == 0) {
            return false;
        }
        file.seekg(0, std::ios::beg);
        char buffer[(int)fsize];
        file.read(buffer, fsize);
        file.close();
        std::string data;
        for (int i = 0; i < (int)fsize; ++i) {
            data.push_back(buffer[i]);
        }
        if (data.empty()) {
            return false;
        }

        // parse the data
        int tagBegin = data.find("ID3");
        if (tagBegin == std::string::npos) {
            return false;
        }
        data = data.substr(tagBegin + 3);
        version[0] = (unsigned short)data[0];
        version[1] = (unsigned short)data[1];
        data = data.substr(2);
        char flags = data[0];
        if (flags & 0x80 == 0x80) { // bit 7 is set
            isSynchronized = true;
        }
        if (flags & 0x40 == 0x40) { // bit 6 is set
            hasExtendedHeader = true;
        }
        if (flags & 0x20 == 0x20) { // bit 5 is set
            isExperimental = true;
        }
        if (flags & 0x10 == 0x10) { // bit 4 is set
            hasFooter = true;
        }
        data = data.substr(1);
        // To calculate tag size: A*2097152+B*16384+C*128+D
        size = (unsigned int)data[0] * 2097152 +
               (unsigned int)data[1] * 16384 +
               (unsigned int)data[2] * 128 +
               (unsigned int)data[3];
        data = data.substr(4);
        title = parseFrame("TIT2", data);
        album = parseFrame("TALB", data);
        artist = parseFrame("TPE1", data);
        year = std::atoi(parseFrame("TORY", data).c_str());

        return true;
    }

    std::string ID3::getTitle() const {
        return title;
    }

    std::string ID3::getArtist() const {
        return artist;
    }

    std::string ID3::getAlbum() const {
        return album;
    }

    int ID3::getYear() const {
        return year;
    }
};