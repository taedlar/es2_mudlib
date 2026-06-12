/*---
description: Gender-related simul_efuns.
author: Annihilator <taedlar@gmail.com>
---*/

string gender_self (mixed subject) {
    switch (objectp(subject) ? subject->query("gender") : subject) {
        case "female":
            return "妳";
        default:
            return "你";
    }
}

string gender_pronoun (string subject) {
    switch (objectp(subject) ? subject->query("gender") : subject) {
        case "deity":
            return "祂";
        case "male":
        case "unknown-gender":
            return "他";
        case "female":
            return "她";
        case "male-animal":
        case "female-animal":
            return "牠";
        default:
            return "它";
    }
}

