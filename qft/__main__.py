from typing import Any, Mapping

from hydra import main


@main(config_path="conf", config_name="qft")
def run(cfg: Mapping[str, Any]) -> None:
    pass


if __name__ == "__main__":
    run()
