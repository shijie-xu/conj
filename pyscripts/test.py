# -*- coding:utf-8 -*-

import nltk
from nltk.corpus import state_union
from nltk.tokenize import PunktSentenceTokenizer

sentence = "GW.Bush is a pig."
# chargement du tokenizer
# you should download data before running this scripts
# set proxy please, if you are in china mainland
# tokenizer = nltk.data.load('tokenizers/punkt/french.pickle')
# sents = "Jadis, une nuit, je fus un papillon, voltigeant, content de son sort. Puis, je m’éveillai, étant Tchouang-tseu. Qui suis-je en réalité ? Un papillon qui rêve qu’il est Tchouang-tseu ou Tchouang qui s’imagine qu’il fut papillon ?"


# print(tokens) 

words = nltk.word_tokenize(sentence)
tagged = nltk.pos_tag(words)

NPGram = r"""NP:NP:{<RB.?>*<VB.?>*<NNP>+<NN>?}"""
chunkParser = nltk.RegexpParser(NPGram)
chunked = chunkParser.parse(tagged)

chunked.draw()