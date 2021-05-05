#include <gtest/gtest.h>
#include <vector>
#include "multimap.h"

/* For this Test Case I am aiming to test functions related to the change 
 * of the container to become a multimap and see if previous accesasble functions work properly
 */

TEST(Multimap, Size){
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }

  EXPECT_EQ(map.Size(), 2);
  map.Insert(2,2);
  EXPECT_EQ(map.Size(),3);

}

TEST(Multimap, GetFunctionGetMin) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  EXPECT_EQ(map.Get(2),2);
  EXPECT_EQ(map.Get(4),4);
  map.Insert(4,2);
  map.Remove(4);
  EXPECT_EQ(map.Get(4),2);  
  
}

TEST(Multimap, Contains) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  EXPECT_EQ(map.Contains(4), true);
}

TEST(Multimap, MaxMin) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  EXPECT_EQ(map.Max(), 4);
  EXPECT_EQ(map.Min(), 2);

}
TEST(Multimap, Insert) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  map.Insert(2,10);
  map.Insert(4,20);
  EXPECT_EQ(map.Size(), 4);
}
TEST(Multimap, Remove) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  map.Insert(2,200);
  map.Remove(2);
  map.Remove(4);
  map.Remove(2);
  EXPECT_EQ(map.Size(), 0);
}

TEST(Multimap, Characters) {
  Multimap<int, char> map;
  std::vector<int> keys{2, 4};

  for (auto i : keys) {
    map.Insert(i, 'a');
  }
  EXPECT_EQ(map.Get(2), 'a');
  EXPECT_EQ(map.Get(4), 'a');

}

TEST(Multimap, Doubles) {
  Multimap<double, double> map;
  std::vector<double> keys{2.2, 4.4};

  for (auto i : keys) {
    map.Insert(i, i);
  }
  EXPECT_EQ(map.Get(2.2), 2.2);
  EXPECT_EQ(map.Get(4.4), 4.4); 
  
}


TEST(Multimap, Errors) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 4};

  EXPECT_THROW(map.Get(2), std::runtime_error);
}



// Test one key
TEST(Multimap, OneKey) {
  Multimap<int, int> map;
  std::vector<int> keys{2};

  for (auto i : keys) {
    map.Insert(i, i);
  }

  EXPECT_EQ(map.Contains(2), true);
  EXPECT_EQ(map.Get(2), keys[0]);
}

// Test multiple keys
TEST(Multimap, MultipleKeys) {
  Multimap<int, int> map;
  std::vector<int> keys{2, 18, 42, 43};

  // Insert a bunch of keys
  for (auto i : keys) {
    map.Insert(i, i);
  }

  // Check that every key is contained
  std::random_shuffle(keys.begin(), keys.end());
  for (auto i : keys) {
    EXPECT_EQ(map.Contains(i), true);
    EXPECT_EQ(map.Get(i), i);
  }
}

 // check if we can get the first value in key using Get Min
 // useful case for homework part 2
TEST(Multimap, GetMin) {
  Multimap<int, int> mmap;
  std::vector<int> keys{2, 18, 42, 43};
  for (auto i : keys) {
    mmap.Insert(i,5); 
  }
  EXPECT_EQ(mmap.Get(mmap.Min()), 5);

}

// Check if we can have duplicates 
// See printed out object to see front and back of container
TEST(Multimap, Duplicates) {
  Multimap<int, int> mmap;
  std::vector<int> keys{2, 18, 42, 43};
  for (auto i : keys) {
    mmap.Insert(i,5); 
  }
  mmap.Insert(2,10);
  mmap.Print();
  mmap.Insert(18,25);
  mmap.Print();
  mmap.Insert(18,40);
  mmap.Print();
  mmap.Remove(18);
  mmap.Print();
  mmap.Remove(18);
  mmap.Print();
  mmap.Remove(18);
  mmap.Print();

}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
