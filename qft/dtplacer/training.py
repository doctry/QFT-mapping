import numpy as np
from gym import Env, spaces


class RoutingEnv(Env):
    def __init__(self, num_qubit: int):
        super(RoutingEnv, self).__init__()

        # observation space
        self.observation_shape = 3 * num_qubit
        self.observation_space = spaces.Box(
            low=np.zeros(self.observation_shape),
            high=np.ones(self.observation_shape),
            dtype=np.float16,
        )
