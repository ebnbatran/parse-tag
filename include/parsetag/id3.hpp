#include <string>
#include <vector>

namespace ParseTag {

    class ID3 {
    public:
        ID3();
        ~ID3() = default;

        /**
         * Parses id3v2 information from the file given
         * in the file path.
         * 
         * @param[in] path
         *     The file path.
         * 
         * @return
         *     An indication whether or not the parsing was
         *     successful.
         */
        bool parseFromFile(const std::string& path);

        /**
         * Gets the song title from the tag.
         * 
         * @return
         *     The song title.
         */
        std::string getTitle() const;

        /**
         * Gets the song artist from the tag.
         * 
         * @return
         *     The song artist.
         */
        std::string getArtist() const;

        /**
         * Gets the song album from the tag.
         * 
         * @return
         *     The song album.
         */
        std::string getAlbum() const;

        /**
         * Gets the year of the song from the tag.
         * 
         * @return
         *     The year of the song. 
         */
        int getYear() const;
    private:
        /**
         * The id3 tag version.
         */
        unsigned short version[2];

        /**
         * The id3 tag flags
         */
        bool isSynchronized;
        bool hasExtendedHeader;
        bool isExperimental;
        bool hasFooter;

        /**
         * The extended id3 tag flags
         */
        bool isUpdate;
        bool hasCRCData;
        bool isRestricted;

        /**
         * The id3 tag size
         */
        unsigned int size;

        /**
         * The song title.
         */
        std::string title;

        /**
         * The song album.
         */
        std::string album;

        /**
         * The song artist.
         */
        std::string artist;

        /**
         * The song year.
         */
        int year;

    private:
        /**
         * Parses a tag frame identified by the given frame ID.
         * 
         * @param[in] frameID
         *     The ID of the requested frame.
         * 
         * @param[in] data
         *     The data of the file.
         * 
         * @return
         *     The text information inside the frame.
         */
        std::string parseFrame(const std::string& frameID, const std::string& data);
    };
};