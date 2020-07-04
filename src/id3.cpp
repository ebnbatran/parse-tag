#include <fstream>
#include <cstdlib>
#include <parsetag/id3.hpp>

namespace ParseTag {

    ID3::ID3() {
        // do nothing
    }

    auto ID3::parseFrame(const std::string& frameID, const std::string& data) -> Frame {
        Frame frame;
        
        // Frame ID
        frame.id = frameID;
        auto frameIDBegin = data.find(frame.id);
        if (frameIDBegin == std::string::npos) {
            return frame;
        }
        auto frameIDEnd = frameIDBegin + 3;
        
        // Frame size
        frame.size = (unsigned int)data[frameIDEnd + 1] * 2097152 +
                     (unsigned int)data[frameIDEnd + 2] * 16384 +
                     (unsigned int)data[frameIDEnd + 3] * 128 +
                     (unsigned int)data[frameIDEnd + 4];
        auto frameSizeEnd = frameIDEnd + 4;
        
        // Frame flags
        frame.flags[0] = (unsigned char)data[frameSizeEnd + 1];
        frame.flags[1] = (unsigned char)data[frameSizeEnd + 2];
        // First byte
        if ((frame.flags[0] & 0x80) == 0x80) { // bit 7 is set
            frame.preservedIfTagAltered = false;
        }
        if ((frame.flags[0] & 0x40) == 0x40) { // bit 6 is set
            frame.preservedIfTagAltered = false;
        }
        if ((frame.flags[0] & 0x20) == 0x20) { // bit 5 is set
            frame.readOnly = true;
        }
        // Second byte
        if ((frame.flags[1] & 0x80) == 0x80) { // bit 7 is set
            frame.compressed = true;
        }
        if ((frame.flags[1] & 0x40) == 0x40) { // bit 6 is set
            frame.encrypted = true;
        }
        if ((frame.flags[1] & 0x20) == 0x20) { // bit 5 is set
            frame.containsGroupingInfo = true;
        }
        auto frameFlagEnd = frameSizeEnd + 2;
        
        // If the frame is a Unique File Identifier
        if (frame.id == "UFID") {
            int state = 0;
            for (int i = frameFlagEnd + 1; i < (frameFlagEnd + frame.size); i++) {
                switch (state) {
                case 0: // Owner Identifier
                    if (data[i] == '\0') {
                        state = 1;
                        continue;
                    }
                    frame.ownerIdentifier.push_back(data[i]);
                case 1: // Identifier
                    frame.identifier.push_back(data[i]);
                default:
                    break; // do nothing
                }
            }
            return frame;
        }

        if (
            (frame.id != "TORY")
        ) { // Text Frame
            
            // Text Encoding
            auto textEncoding = (unsigned char)data[frameFlagEnd + 1];
            switch (textEncoding) {
            case 0x00:
                frame.isISO = true;
                break;
            case 0x01:
                frame.isUnicode = true;
                break;
            default:
                break; // do nothing
            }

            // Frame info
            std::string contentString;
            for (int i = frameFlagEnd + 2; i < (frameFlagEnd + frame.size); i++) {
                if (
                    (data[i] < ' ') || 
                    (data[i] > '~')
                ) {
                    continue;
                }
                contentString.push_back(data[i]);
            }
            
            frame.info = contentString;
        } else {
            std::string contentString;
            for (int i = frameFlagEnd + 1; i < (frameFlagEnd + frame.size); i++) {
                if (
                    (data[i] < ' ') || 
                    (data[i] > '~')
                ) {
                    continue;
                }
                contentString.push_back(data[i]);
            }
            
            frame.info = std::atoi(contentString.c_str());
        }

        return frame;
    }

    bool ID3::parseFromFile(const std::string& path) {
        // Open the file and read its contents
        // open file
        std::ifstream file{path};
        if (!file.is_open()) {
            return false;
        }
        // seek till the end and get the file size
        file.seekg(0, std::ios::end);
        std::streamsize fsize = file.tellg();
        if (fsize == 0) {
            return false;
        }
        // rewind to the beginning and read whole file into buffer
        file.seekg(0, std::ios::beg);
        char buffer[(int)fsize];
        file.read(buffer, fsize);
        // close file
        file.close();
        // store the buffer into a std::string object
        std::string data;
        for (int i = 0; i < (int)fsize; ++i) {
            data.push_back(buffer[i]);
        }
        if (data.empty()) {
            return false;
        }

        // Parse the data
        // find the ID3 Tag
        int tagBegin = data.find("ID3");
        if (tagBegin == std::string::npos) {
            return false;
        }
        data = data.substr(tagBegin + 3);
        // find the version
        version[0] = (unsigned short)data[0];
        version[1] = (unsigned short)data[1];
        data = data.substr(2);
        // find the tag flags
        char flags = data[0];
        if ((flags & 0x80) == 0x80) { // bit 7 is set
            isSynchronized = true;
        }
        if ((flags & 0x40) == 0x40) { // bit 6 is set
            hasExtendedHeader = true;
        }
        if ((flags & 0x20) == 0x20) { // bit 5 is set
            isExperimental = true;
        }
        if ((flags & 0x10) == 0x10) { // bit 4 is set
            hasFooter = true;
        }
        data = data.substr(1);
        // calculate tag size using this formula: A*2097152+B*16384+C*128+D
        size = (unsigned int)data[0] * 2097152 +
               (unsigned int)data[1] * 16384 +
               (unsigned int)data[2] * 128 +
               (unsigned int)data[3];
        data = data.substr(4);
        // parse the tag info
        title = parseFrame("TIT2", data);
        album = parseFrame("TALB", data);
        artist = parseFrame("TPE1", data);
        year = parseFrame("TORY", data);

        return true;
    }

    std::string ID3::getTitle() const {
        return std::get<std::string>(title.info);
    }

    std::string ID3::getArtist() const {
        return std::get<std::string>(artist.info);
    }

    std::string ID3::getAlbum() const {
        return std::get<std::string>(album.info);
    }

    int ID3::getYear() const {
        return std::get<int>(year.info);
    }
};
