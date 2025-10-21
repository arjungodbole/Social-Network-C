class Solution
{
public:
    int numIslands(vector<vector<char>> &grid)
    {
        int numIslands = 0;
        int rows = grid.size();
        int cols = grid[0].size();

        vector<pair<int, int>> different = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        vector<vector<bool>> vis(rows, vector<bool>(cols, 0));

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                // simple BFS
                if (grid[i][j] == '1' && !vis[i][j])
                {
                    ++numIslands;
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    vis[i][j] = true;

                    while (!q.empty())
                    {
                        pair<int, int> p = q.front();
                        q.pop();

                        for (int k = 0; k < different.size(); k++)
                        {
                            int differentRow = different[k].first;
                            int differentColumn = different[k].second;
                            int newRow = p.first + differentRow;
                            int newCol = p.second + differentColumn;

                            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && grid[newRow][newCol] == '1' && !vis[newRow][newCol])
                            {
                                vis[newRow][newCol] = true;
                                q.push({newRow, newCol});
                            }
                        }
                    }
                }
            }
        }
        return numIslands;
    }
};