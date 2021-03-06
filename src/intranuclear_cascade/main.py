""" Main driver code. """

from tqdm import tqdm
from .utilities import logger

from . import run_modes
from .config import settings


class IntranuclearCascade:
    """ Main driver class for the IntranuclearCascade code. """
    def __init__(self, run_card):
        settings().load(run_card)
        self.nevents = settings().nevents
        self.calc = run_modes.RunMode(settings().get_run('mode', 'generate'))

    def run(self):
        """ Generate events for the given input settings. """
        events = []
        for _ in tqdm(range(self.nevents), ncols=80):
            event = self.calc.generate_one_event()
            events.append(event)

        return self.calc.finalize(events)


def intranuclear_cascade():
    """ External entry point. """
    logger.init('cascade', 'cascade.log')
    driver = IntranuclearCascade('run.yml')
    driver.run()


if __name__ == '__main__':
    cascade()
