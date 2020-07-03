#include <gtest/gtest.h>

#include <parsetag/id3.hpp>

TEST(ID3Tests, parseFromFile) {
    ParseTag::ID3 id3;
    
    ASSERT_TRUE(id3.parseFromFile("akon-right-now.mp3"));
    EXPECT_EQ(id3.getTitle(), "Right Now (Na Na Na)");
    EXPECT_EQ(id3.getArtist(), "Akon");
    EXPECT_EQ(id3.getAlbum(), "Freedom");
    EXPECT_EQ(id3.getYear(), 2008);
}