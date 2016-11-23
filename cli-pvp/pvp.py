#!/usr/bin/env python

# --global commands define

valid_commands = {
    'z': (1, {'dmg': 1, 'rng': (0, 1), 'kd': lambda x: x + 1, 'recover': 1}),
    'x': (2, {'gethit': lambda ca, cd: (ca, cd.go(cd.pos + ca.tw)), 'dmg': 2, 'rng': (0, 1), 'kd': lambda x: x + 1, 'recover': 2}, {}),
    '66z': (2, {'moving': lambda c: c.go(c.pos + 2).towords(1), 'dmg': 1, 'rng': (0, 1, 2), 'kd': lambda x: x + 2, 'recover': 2}, {}),
    '66x': (1, {'moving': lambda c: c.go(c.pos + 1).towords(1), 'gethit': lambda ca, cd: (ca, cd.fly()), 'dmg': 1, 'rng': (1,), 'kd': lambda x: x + 1, 'recover': 3}),
    '44z': (2, {'moving': lambda c: c.go(c.pos - 2).towords(-1), 'dmg': 1, 'rng': (0, 1, 2), 'kd': lambda x: x + 2, 'recover': 2}, {}),
    '44x': (1, {'moving': lambda c: c.go(c.pos - 1).towords(-1), 'gethit': lambda ca, cd: (ca, cd.fly()), 'dmg': 1, 'rng': (1,), 'kd': lambda x: x + 1, 'recover': 3}),
    '8z': (2, {'moving': lambda c: c.fly(), 'dmg': 1, 'rng': (0, 1), 'kd': lambda x: x + 2, 'recover': 2}, {'dmg': 1, 'rng': (0, 1), 'kd': lambda x: x + 2, 'recover': 1}),
    '8x': (2, {'moving': lambda c: c.fly(), 'dmg': 2, 'rng': (0,), 'kd': lambda x: x - 3, 'recover': 2}, {'dmg': 5, 'rng': (0,), 'kd': lambda: 20}),
    '668z': (2, {'moving': lambda c: c.fly().go(c.pos + 1).towords(1), 'dmg': 1, 'rng': (1, 2), 'kd': lambda x: x + 3, 'recover': 2}, {'dmg': 1, 'rng': (1,), 'kd': 3, 'recover': 1}),
    '448z': (2, {'moving': lambda c: c.fly().go(c.pos - 1).towords(-1), 'dmg': 1, 'rng': (1, 2), 'kd': lambda x: x + 3, 'recover': 2}, {'dmg': 1, 'rng': (1,), 'kd': 3, 'recover': 1}),
    '66': (1, {'moving': lambda c: c.go(c.pos + 1).towords(1)}),
    '44': (1, {'moving': lambda c: c.go(c.pos - 1).towords(-1)}),
    '8': (1, {'moving': lambda c: c.fly()}),
    '668': (1, {'moving': lambda c: c.fly().go(c.pos + 1).towords(1)}),
    '448': (1, {'moving': lambda c: c.fly().go(c.pos - 1).towords(-1)}),
    '6': (0, {'moving': lambda c: c.towords(1)}),
    '4': (0, {'moving': lambda c: c.towords(-1)}),
    's': (1, {'moving': lambda c: c})
}


class Character:
    kd = 0  # kock down value
    tw = 1  # towords, 1 -> -1 <-
    pos = 0
    air = False
    invc = 0
    health = 100
    stun = 3  # stun time
    acting = 0  # there are many steps for 1 action

    def __init__(self, name):
        self.name = name

    def lock(self, time):
        self.stun = time
        return self

    def invincible(self, time):
        self.invc = time
        return self

    def go(self, pos):
        self.pos = pos
        return self

    def fly(self):
        self.air = True
        return self

    def towords(self, tw):
        self.tw = tw
        return self


class Match:
    stage_size = 30
    _player1 = Character('p1')
    _player1.pos = 10
    _player2 = Character('p2')
    _player2.pos = 30
    _player2.tw = -1
    _lplayer = _player1
    _rplayer = _player2
    _ticks = 100

    def remove_useless(self, cmdlist):
        for i, s in enumerate(cmdlist):  # only cmds in dict
            if s not in valid_commands:
                del cmdlist[i]

    def calc_tick(self, p1cmdlist, p2cmdlist):  # must be lists

        p1cmd = valid_commands.get(p1cmdlist[0], (1, {}))
        p2cmd = valid_commands.get(p2cmdlist[0], (1, {}))
        # continue the unfinished action first:
        if self._player1.acting > 0:
            pass
        if self._player2.acting > 0:
            pass

        # adjust towords
        if not self._player1.stun > 0:
            while p1cmd[0] == 0:  # a zero-time action
                p1cmd[1].get('moving', lambda x: x)(self._player1)  # get the moving action then call the action function
                del p1cmdlist[0]  # and remove it from commands list
                p1cmd = valid_commands.get(p1cmdlist[0], (1, {}))
        if not self._player2.stun > 0:
            while p2cmd[0] == 0:  # a zero-time action
                p2cmd[1].get('moving', lambda x: x)(self._player2)  # get the moving action then call the action function
                del p2cmdlist[0]  # and remove it from commands list
                p2cmd = valid_commands.get(p2cmdlist[0], (1, {}))

        self._player1.acting = p1cmd[0]  # get acting status
        self._player2.acting = p2cmd[0]
        # 1. save current pos
        pos1 = self._player1.pos
        pos2 = self._player2.pos

        #  2. move characters
        if not self._player1.stun > 0:
            p1cmd[1].get('moving', lambda x: x)(self._player1)  # call moving function
        if not self._player2.stun > 0:
            p2cmd[1].get('moving', lambda x: x)(self._player2)
        if self._lplayer.pos > self._rplayer.pos:
            if (self._lplayer.air ^ self._rplayer.air) or (p1cmdlist[0] in ('44z', '66z')) or (p2cmdlist[0] in ('44z', '66z')):  # not at same y or someone using dash-z
                self._lplayer, self._rplayer = self._rplayer, self._lplayer  # exchg left and right, able to walk through
            else:
                mid = (self._player1.pos + self._player2.pos) / 2  # prevent  crossing
                self._player1.pos = self._player2.pos = mid  # they hit and stay at same pos
        self._player1.lock(p1cmd[0] - 1)  # befor calc hit stun by the opponent, make self stun first
        self._player2.lock(p2cmd[0] - 1)

        # 3. calc if hit
        if not (self._player1.air ^ self._player2.air):  # same y
            p1atkrng = map(lambda x: x * self._player1.tw + pos1, p1cmd[1].get('rng', []))
            p2atkrng = map(lambda x: x * self._player2.tw + pos2, p2cmd[1].get('rng', []))
            p1acting = self._player1.acting
            p2acting = self._player2.acting
            if self._player2.pos in p1atkrng and p1acting:
                if not self._player2.invc > 0:  # if not invincible
                    self._player2.health -= p1cmd[1].get('dmg', 0)  # 1) decrease health
                    p1cmd[1].get('gethit', lambda ca, cd: (ca, cd))(self._player1, self._player2)  # 2) call get hit action
                    self._player2.lock(p1cmd[1].get('recover', 0))  # 3) make hit stun
                    self._player2.acting = 0  # 3.5) interrupt p2 actions but not allow this time
                    self._player2.kd = p1cmd[1].get('kd', lambda x: x)(self._player2.kd)  # 4) adjust knock down value

            if self._player1.pos in p2atkrng and p2acting:
                if not self._player1.invc > 0:  # not invincible
                    self._player1.health -= p2cmd[1].get('dmg', 0)  # 1) decrease health
                    p2cmd[1].get('gethit', lambda ca, cd: (ca, cd))(self._player2, self._player1)  # 2) call get hit action
                    self._player1.lock(p2cmd[1].get('recover', 0))  # 3) make hit stun
                    self._player1.acting = 0  # 3.5) interrupt p1 actions but not allow this time
                    self._player1.kd = p2cmd[1].get('kd', lambda x: x)(self._player1.kd)  # 4) adjust knock down value

        # 4. check if kd touched the limit
        if self._player1.kd >= 20:
            self._player1.lock(2)
            self._player1.invincible(5)
        if self._player2.kd >= 20:
            self._player2.lock(2)
            self._player2.invincible(5)

        # 5. adjust properties that automaticly changing
        self._ticks -= 1
        self._player1.invc -=1
        self._player2.invc-=1
        self._player1.air = False
        self._player2.air = False
        self._player1.acting -= 1
        self._player2.acting -= 1
        self._player1.stun -= 1
        self._player2.stun -= 1
        pass


cmds = ''
m = Match()
p1cmdlist = []
p2cmdlist = []
while cmds != 'q':
    # cmds = raw_input('->')
    cmds = '4,qcc,6,4,6,44z,668z'
    p1cmdlist += cmds.split(',')
    # cmds = raw_input('<=')
    cmds = 'aab,6,cc,4,66z,8x'
    p2cmdlist += cmds.split(',')
    m.remove_useless(p1cmdlist)
    m.remove_useless(p2cmdlist)
    m.calc_tick(p1cmdlist, p2cmdlist)
