#include <bits/stdc++.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    TreeNode* mirrorTree(TreeNode* root) {
      if (root) {
        swap(root->left, root->right);
        mirrorTree(root->left);
        mirrorTree(root->right);
      }
      return root;
    }
};

int main(int argc, char *argv[])
{
    return 0;
}
