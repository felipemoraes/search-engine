

#include "../lib/common/graph.h"

#include <gtest/gtest.h>

using namespace htmlcxx;

class GraphTest : public ::testing::Test {
protected:
    
    virtual void SetUp(){
        // A -> B,F
        // B -> C,D,E,F
        // C -> E,F
        // D -> A,C,E, F
        // E -> A
        // F -> A, B, E
        graph = new Graph(6);
        graph->insert(0, 1);
        graph->insert(0, 5);
        graph->insert(1, 2);
        graph->insert(1, 3);
        graph->insert(1, 4);
        graph->insert(1, 5);
        graph->insert(2, 4);
        graph->insert(2, 5);
        graph->insert(3, 0);
        graph->insert(3, 2);
        graph->insert(3, 4);
        graph->insert(3, 5);
        graph->insert(4, 0);
        graph->insert(5, 0);
        graph->insert(5, 1);
        graph->insert(5, 4);
        graph->compute_out_links();
    }
  
    virtual void TearDown(){
    }
    
    Graph* graph;
    
};


TEST_F(GraphTest, testSize){
    EXPECT_EQ(graph->get_size(), 6);
}

TEST_F(GraphTest, testInlinks){
    vector<int> inlinks = graph->get_inlinks(1);
    EXPECT_EQ(inlinks.size(), 2);
}

TEST_F(GraphTest, testNoOutlinks){
    unordered_set<int>* outlinks = graph->get_no_outlinks();
    EXPECT_EQ(outlinks->size(), 0);
}


TEST_F(GraphTest, testOutlinksCount){
    EXPECT_EQ(graph->get_outlink_count(0), 2);
    EXPECT_EQ(graph->get_outlink_count(1), 4);
    EXPECT_EQ(graph->get_outlink_count(2), 2);
    EXPECT_EQ(graph->get_outlink_count(3), 4);
    EXPECT_EQ(graph->get_outlink_count(4), 1);
    EXPECT_EQ(graph->get_outlink_count(5), 3);
    vector<float>* result = graph->pagerank(100);
    for (auto rank: *result) {
        cout << rank << endl;
    }

    
}
