"""Print one event (and everything nested in it) from the dumps, or find where something happens.

    python3 show.py 13.2.5                 the event/group 13.2.5 of Game_events and all its sub-events
    python3 show.py 13.2.5 Menu_Events     ... of another sheet
    python3 show.py -f Player_get_hit      where a Function is defined and every place that calls it
    python3 show.py -g 'body_1'            grep all sheets (regex), printing sheet:path lines
"""
import re
import signal
import sys
from pathlib import Path

EV = Path(__file__).resolve().parent / 'events'
SHEETS = ['Game_events', 'Menu_Events', 'Login_events', 'Loading_events', 'Everywere', 'Tutorial_events']


def lines(sheet):
    return (EV / ('%s.txt' % sheet)).read_text().split('\n')


def main(a):
    if not a:
        sys.exit(__doc__)
    if a[0] in ('-f', '-g'):
        rx = re.compile(r'OnFunction\("%s"\)|CallFunction\("%s"|Function\.Call\("%s"' % ((re.escape(a[1]),) * 3)) if a[0] == '-f' else re.compile(a[1])
        for sh in SHEETS:
            for ln in lines(sh):
                if rx.search(ln):
                    print('%-16s %s' % (sh, ln))
        return
    path, sheet = a[0], (a[1] if len(a) > 1 else 'Game_events')
    out = [ln for ln in lines(sheet) if ln.startswith(path + ' ') or ln.startswith(path + '.')]
    print('\n'.join(out) if out else 'no event %s in %s' % (path, sheet))


if __name__ == '__main__':
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)   # | head
    main(sys.argv[1:])
