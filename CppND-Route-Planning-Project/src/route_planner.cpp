#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.

    // My Code Start
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
    // My Code End

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    // My Code Start
    return (*node).distance((*end_node));
    // My Code End

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    // My Code Start
    // Find neighbors in node (this method only finds new neighbors), store in neighbors attribute: std::vector<Node *> neighbors
    (*current_node).FindNeighbors();
    // Loop through current_node.neighbors, set the parent, the h_value, and the g_value
    for(auto & node : current_node->neighbors)
    {
        node->parent = current_node;
        node->h_value = CalculateHValue(node);
        node->g_value = current_node->g_value + (*current_node).distance((*node));
        node->visited = true;
        open_list.emplace_back(node);
    }
    // My Code End
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool wayToSort(RouteModel::Node *node_i, RouteModel::Node *node_j){
    // bool wayToSort(int i, int j) { return i > j; }
    // Flipping sign to get smallest at back
    return (node_i->g_value + node_i->h_value) < (node_j->g_value + node_j->h_value);
}


RouteModel::Node *RoutePlanner::NextNode() 
{

    // Sort open_list
    std::sort(open_list.begin(), open_list.end(), wayToSort);  // Create method for sorting by f
    // Point to lowest sum
    RouteModel::Node *smallest_node;
    smallest_node = open_list[-1];
    // Remove node from list
    open_list.pop_back();
    // My Code End
    return smallest_node;

}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    // Store new node here
    RouteModel::Node *new_node;

    // Add current_node to path prior to looping through parents
    // Loop until new_node == start_node  Do they have equivalent method?
    while(true)
    {
        // Add current node to the path
        path_found.push_back(*current_node);
        // Test for starting node, if found then break
        if((current_node->x == start_node->x) && (current_node->y == start_node->y)){break;}
        // set new_node = parent
        new_node = current_node->parent;
        // compute and add distance between 
        distance += new_node->distance(*current_node);
        // Update current node
        current_node = new_node;
    }

    // Reverse path order
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    // Set the current_node to the start_node
    current_node = start_node;
    // Add neighbors to initialize frontier
    AddNeighbors(current_node);
    // Loop until goal is found
    while(true)
    {
        // Get next node: NextNode()
        current_node = NextNode();
        // Test for goal
        if((current_node->x == end_node->x) && (current_node->y == end_node->y)) break;
        // Add neighbors
        AddNeighbors(current_node);
    }

    // Store path in m_Model.path return path from ConstructFinalPath and pass it the final node (goal node)
    m_Model.path = ConstructFinalPath(current_node);
}