import sys

filename = sys.argv[1]

nums = []
adj_list = []

with open(filename, 'r') as f:
    for x in f:
        x = x.split()
        for y in x: 
            nums.append(int(y))

count = 0
num = nums[0]
for i in range(num):
    count += 1
    id = nums[count]
    assert i == id, (i, id)

    count += 1
    num_adj = nums[count]
    adj = []
    for _ in range(num_adj):
        count += 1
        adj.append(nums[count])
    adj_list.append(adj)

assert len(adj_list) == num, adj_list

for (i, adj) in enumerate(adj_list):
    for j in adj:
        assert i in adj_list[j], (i, j, adj_list)
