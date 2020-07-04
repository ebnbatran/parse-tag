#include <gtest/gtest.h>
#include <parsetag/id3.hpp>

TEST(ID3Tests, parseFromFile1) {
    ParseTag::ID3 id3;
    
    ASSERT_TRUE(id3.parseFromFile("akon-right-now.mp3"));
    EXPECT_EQ(id3.getTitle(), "Right Now (Na Na Na)");
    EXPECT_EQ(id3.getArtist(), "Akon");
    EXPECT_EQ(id3.getAlbum(), "Freedom");
    EXPECT_EQ(id3.getYear(), 2008);
}

TEST(ID3Tests, parseFromFile2) {
    ParseTag::ID3 id3;
    
    ASSERT_TRUE(id3.parseFromFile("miley-cyrus-the-climb.mp3"));
    EXPECT_EQ(id3.getTitle(), "The Climb");
    EXPECT_EQ(id3.getArtist(), "Miley Cyrus");
    EXPECT_EQ(id3.getAlbum(), "The Climb");
    EXPECT_EQ(id3.getYear(), 2009);
}