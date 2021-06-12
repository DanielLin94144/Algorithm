import torch
# def one_hot_embedding(labels, num_classes):
#     """Embedding labels to one-hot form.

#     Args:
#       labels: (LongTensor) class labels, sized [N,].
#       num_classes: (int) number of classes.

#     Returns:
#       (tensor) encoded labels, sized [N, #classes].
#     """
#     y = torch.eye(num_classes) 
#     return y[labels] 

# print(one_hot_embedding(9, 10))


# m = torch.nn.Conv1d(16, 33, 3, padding = 3//2)
# input = torch.randn(20, 16, 50)
# output = m(input)
# print(output.size())

# prob = torch.tensor([[0.0029, 0.0241, 0.0051, 0.0038, 0.0006, 0.0019, 0.0018, 0.0061, 0.0026,                                                                                                                 
#         0.0027, 0.0046, 0.0047, 0.0047, 0.0003, 0.0019, 0.0087, 0.0059, 0.0620,                                                                                                                 
#         0.0039, 0.0132, 0.0033, 0.0013, 0.0039, 0.0022, 0.0007, 0.0036, 0.0013,                                                                                                                 
#         0.0015, 0.0007, 0.0011, 0.0060, 0.0120, 0.0005, 0.0296, 0.0009, 0.0535,                                                                                                                 
#         0.0038, 0.0025, 0.0051, 0.0008, 0.0016, 0.0009, 0.0022, 0.0010, 0.0024,                                                                                                                 
#         0.1127, 0.0017, 0.0021, 0.0058, 0.0007, 0.0006, 0.2523, 0.0062, 0.0093,                                                                                                                 
#         0.0018, 0.0062, 0.0081, 0.0011, 0.0008, 0.0036, 0.0100, 0.0005, 0.0041,                                                                                                                 
#         0.0012, 0.0009, 0.0082, 0.0005, 0.0007, 0.0011, 0.2355, 0.0204], 
#         [0.0029, 0.0241, 0.0051, 0.0038, 0.0006, 0.0019, 0.0018, 0.0061, 0.0026,                                                                                                                 
#         0.0027, 0.0046, 0.0047, 0.0047, 0.0003, 0.0019, 0.0087, 0.0059, 0.0620,                                                                                                                 
#         0.0039, 0.0132, 0.0033, 0.0013, 0.0039, 0.0022, 0.0007, 0.0036, 0.0013,                                                                                                                 
#         0.0015, 0.0007, 0.0011, 0.0060, 0.0120, 0.0005, 0.0296, 0.0009, 0.0535,                                                                                                                 
#         0.0038, 0.0025, 0.0051, 0.0008, 0.0016, 0.0009, 0.0022, 0.0010, 0.0024,                                                                                                                 
#         0.1127, 0.0017, 0.0021, 0.0058, 0.0007, 0.0006, 0.2523, 0.0062, 0.0093,                                                                                                                 
#         0.0018, 0.0062, 0.0081, 0.0011, 0.0008, 0.0036, 0.0100, 0.0005, 0.0041,                                                                                                                 
#         0.0012, 0.0009, 0.0082, 0.0005, 0.0007, 0.0011, 0.2355, 0.0204]])
# prob = torch.rand(32, 260, 71)
# prob = prob.mean(dim = 1)
# entropy = -torch.sum(prob * torch.log(prob + 1e-7), dim=-1)
# print(entropy)
# entropy = entropy.mean(0)
# print(entropy)

# prob = torch.rand(32, 520, 71)
# prob = prob.mean(dim = 1)
# entropy = -torch.sum(prob * torch.log(prob + 1e-7), dim=-1)
# entropy = entropy.mean(0)
# print(entropy)

x = torch.tensor([0, 1, 2, 0, 3, 4, 0, 5, 5, 5, 3, 3])
idx = torch.where(x == 0, False, True)

x = x[idx]
print(x)
print(idx)
ir_idx = ~idx
print((~idx).sum())
x, count = torch.unique_consecutive(x, return_counts=True)
print(x)
print(count - 1)