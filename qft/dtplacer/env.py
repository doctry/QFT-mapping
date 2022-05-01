MAX_NUM_QUBIT = 1280

from ..decision_transformer import *


def get_batch(batch_size=256, max_num_qubit=MAX_NUM_QUBIT):

    return s, a, r, d, rtg, timesteps, mask


if model_type == "dt":
    model = DecisionTransformer(
        state_dim=state_dim,
        act_dim=act_dim,
        max_length=K,
        max_ep_len=max_ep_len,
        hidden_size=variant["embed_dim"],
        n_layer=variant["n_layer"],
        n_head=variant["n_head"],
        n_inner=4 * variant["embed_dim"],
        activation_function=variant["activation_function"],
        n_positions=1024,
        resid_pdrop=variant["dropout"],
        attn_pdrop=variant["dropout"],
    )
elif model_type == "bc":
    model = MLPBCModel(
        state_dim=state_dim,
        act_dim=act_dim,
        max_length=K,
        hidden_size=variant["embed_dim"],
        n_layer=variant["n_layer"],
    )
else:
    raise NotImplementedError

model = model.to(device=device)

warmup_steps = variant["warmup_steps"]
optimizer = torch.optim.AdamW(
    model.parameters(),
    lr=variant["learning_rate"],
    weight_decay=variant["weight_decay"],
)
scheduler = torch.optim.lr_scheduler.LambdaLR(
    optimizer, lambda steps: min((steps + 1) / warmup_steps, 1)
)

if model_type == "dt":
    trainer = SequenceTrainer(
        model=model,
        optimizer=optimizer,
        batch_size=batch_size,
        get_batch=get_batch,
        scheduler=scheduler,
        loss_fn=lambda s_hat, a_hat, r_hat, s, a, r: torch.mean((a_hat - a) ** 2),
        eval_fns=[eval_episodes(tar) for tar in env_targets],
    )
elif model_type == "bc":
    trainer = ActTrainer(
        model=model,
        optimizer=optimizer,
        batch_size=batch_size,
        get_batch=get_batch,
        scheduler=scheduler,
        loss_fn=lambda s_hat, a_hat, r_hat, s, a, r: torch.mean((a_hat - a) ** 2),
        eval_fns=[eval_episodes(tar) for tar in env_targets],
    )

if log_to_wandb:
    wandb.init(
        name=exp_prefix,
        group=group_name,
        project="decision-transformer",
        config=variant,
    )
    # wandb.watch(model)  # wandb has some bug

for iter in range(variant["max_iters"]):
    outputs = trainer.train_iteration(
        num_steps=variant["num_steps_per_iter"], iter_num=iter + 1, print_logs=True
    )
    if log_to_wandb:
        wandb.log(outputs)
