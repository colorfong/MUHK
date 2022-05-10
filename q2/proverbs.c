#include <sys/types.h>
#include <unistd.h>

char *proverbs[] = {
  "Absolute power corrupts absolutely.",
  "An apple a day keeps the doctor away.",
  "Don't count your chickens before they are hatched.",
  "Eat, drink and be merry, for tomorrow we die.",
  "Every cloud has a silver lining.",
  "Fight fire with fire.",
  "If anything can go wrong, it will.",
  "Honesty is the best policy.",
  "If you can't stand the heat get out of the kitchen.",
  "If a job is worth doing it is worth doing well.",
  "History repeats itself.",
  "Home is where the heart is.",
  "It is easy to be wise after the event.",
  "It takes two to tango.",
  "Life begins at forty.",
  "Look before you leap.",
  "Marry in haste, repent at leisure.",
  "Many hands make light work.",
  "Money isn't everything.",
  "Necessity is the mother of invention.",
  "Never let the sun go down on your anger.",
  "Nothing is certain but death and taxes.",
  "One hand washes the other.",
  "Only fools and horses work.",
  "Oil and water don't mix.",
  "Revenge is a dish best served cold.",
  "Seek and ye shall find.",
  "The darkest hour is just before the dawn.",
  "The leopard does not change his spots.",
  "The opera ain't over till the fat lady sings.",
  "The longest journey starts with a single step.",
  "The price of liberty is eternal vigilance.",
  "There's more than one way to skin a cat.",
  "There's no smoke without fire.",
  "They that sow the wind, shall reap the whirlwind.",
  "Time is money.",
  "Two is company, but three's a crowd.",
  "Why keep a dog and bark yourself?",
  "You are never too old to learn.",
  "Youth is wasted on the young."
};

char *get_proverb() {
    return proverbs[getpid() % 40];
}

