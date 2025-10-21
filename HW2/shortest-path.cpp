class Solution
{
public:
    int shortestPathBinaryMatrix(vector<vector<int>> &grid)
    {
        int n = grid.size();
        // base cases
        if (n == 0)
        {
            return -1;
        }
        if (grid[0][0] == 1)
        {
            return -1;
        }
        // single clear cell
        if (n == 1)
        {
            return 1;
        }

        if (grid[n - 1][n - 1] == 1)
        {
            return -1;
        }

        int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
        int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};

        queue<pair<int, int>> q;
        // still need something to store into the graph
        q.push({0, 0});
        grid[0][0] = 1;

        while (q.size() > 0)
        {
            // makes two variables for p
            auto p = q.front();
            q.pop();
            int first = p.first;
            int second = p.second;
            int distance = grid[first][second];

            if (first == n - 1 && second == n - 1)
            {
                return distance;
            }

            for (int i = 0; i < 8; i++)
            {
                int nx = first + dx[i];
                int ny = second + dy[i];
                // all of the checks to make sure that bounds and cell values are valid
                if (nx >= 0 && ny >= 0 && nx < n && ny < n && grid[nx][ny] == 0)
                {
                    grid[nx][ny] = distance + 1;
                    q.push({nx, ny});
                }
            }
        }
        return -1;
    }
};