Models aim to predict a given wet signal from the dry, black box modeling an existing fx chain. I decided to use Neutone SDK even if I have no experience in PyTorch, cause it uses auraloss, a collection of very useful loss functions for audio tasks. It has got a wrapper that allows to put the model in a plugin and try it in a real time setting, very handy.

You can find some samples in wav folder (clean = dry, effect = wet):
- audio is a folder with some recs to try models, also loop.wav is a rec I use as a test
- harmonic: make a guitar sounds like every note is a natural harmonic
- dirty_rec: make a dirty rec from a device in a noisy room sounds like an amp
- gtr_crusher: make a guitar sounds like my plugin
