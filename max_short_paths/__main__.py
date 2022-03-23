from hydra import main
from omegaconf import DictConfig


@main(config_path="conf", config_name="qft")
def app(cfg: DictConfig):
    pass


if __name__ == "__main__":
    pass
