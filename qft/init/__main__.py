from typing import Any

import hydra
import torch
from omegaconf import DictConfig
from torch.nn import Linear, Module
from torch.nn import functional as F
from torch.nn import utils
from torch.optim import AdamW
from torch.utils.data import DataLoader
from transformers import AutoModel

from .datasets import DataGen, FixedLenDataGen
from .progress import progress


class EmbeddingsWrapper(Module):
    def __init__(self, emb) -> None:
        super().__init__()
        self.emb = emb
        self.linear = Linear(769, 768)

    def forward(self, ipt, *args, **kwargs):
        (qubit, total_time) = ipt

        emb = self.emb(qubit)
        emb = torch.cat([emb, total_time], dim=1)

        return self.linear(emb)


class DecisionTransformer(Module):
    def __init__(self, model) -> None:
        super().__init__()
        self.model = model
        self.model.embeddings = EmbeddingsWrapper(self.model.embeddings)

    def forward(self, qubit, total_time, *args, **kwargs) -> Any:
        return self.model((qubit, total_time), *args, **kwargs)


@hydra.main("conf", "main")
def run(cfg: DictConfig):
    init_cfg = cfg["init"]
    device = init_cfg["device"]

    model = AutoModel.from_pretrained(init_cfg["model_name"]).to(device)

    for p in model.parameters():
        p.requires_grad_(False)

    method = init_cfg["method"]

    dataset = FixedLenDataGen()
    dataloader = DataLoader(
        dataset, batch_size=init_cfg["batch_size"], num_workers=init_cfg["workers"]
    )
    q_device = int(init_cfg["q_device"])

    if method == "decision-transformer":
        model = DecisionTransformer(model)
    else:
        raise ValueError

    optimizer = AdamW(model.parameters())

    causal_mask = torch.tril(torch.ones([1, q_device, q_device]))

    for (step, (qubit, total_time)) in enumerate(progress(dataloader)):
        qubit = qubit + 100

        qubit = torch.tensor(qubit, device=device)
        total_time = torch.tensor(total_time, device=device)

        if step == init_cfg["requires_grad"]:
            for p in model.parameters():
                p.requires_grad_()

        output = model(qubit, total_time, attention_mask=causal_mask)
        shifted = torch.cat([torch.zeros([1, 1], device=device), qubit])[:, :-1]

        loss = F.cross_entropy(output, shifted)

        optimizer.zero_grad()

        loss.backward()
        utils.clip_grad_norm_(model.parameters())

        optimizer.step()


if __name__ == "__main__":
    run()
