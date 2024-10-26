import { parseArgs } from "util";

const { values } = parseArgs({
  args: Bun.argv,
  options: {
    script: {
      type: 'string',
    },
  },
  strict: true,
  allowPositionals: true,
});

interface ExitNode {
  actionType: "exit"
  text: string,
}

interface TextNode {
  actionType: "text"
  text: string;
  nextNode: string;
}

interface ResponseNode {
  actionType: "response"
  text: string;
  responses: string[];
}

// TODO some sort of item id or something?
interface ItemNode {
  actionType: "getItem";
  text: string;
  nextNode: string;
}

interface FlagNode {
  actionType: "flag",
  flag: string,
  nextNode: string;
}

type DialogueNode = ExitNode | TextNode | ResponseNode | ItemNode | FlagNode;

interface ResponseEdge {
  responseText: string;
  nextNode: string;
}

interface Conversation  {
  npcName: string;
  dialogueNodes: Record<string, DialogueNode>;
  responseEdges: Record<string, ResponseEdge>;
}

const testConv: Conversation = {
  npcName: "Fishy McGills",
  dialogueNodes: {
    entry: {
      actionType: "response",
      text: "Hello",
      responses: [
        "goodbye",
        "hi",
      ],
    },
    story: {
      actionType: "response",
      text: "Would you like a fish stick?",
      responses: [
        "goodbye",
        "getFishStick",
        "quest",
        "what",
      ]
    },
    repeat: {
      actionType: "response",
      text: "I SAID would you like a fish stick?",
      responses: [
        "goodbye",
        "getFishStick",
        "what",
        "first",
      ]
    },
    getFishstick: {
      actionType: "getItem",
      text: "You Received a ~~~Fish stick (Tuna)~~~",
      nextNode: "story",
    },
    quest: {
      actionType: "flag",
      flag: "mayorQuestUpdated",
      nextNode: "questExit",
    },
    questExit: {
      actionType: "exit",
      text: "He's in the fish mines! GO AT ONCE!!!"
    },
    exit: {
     actionType: "exit",
     text: "Hope to sea you around!",
    }
  },
  responseEdges: {
      goodbye: {responseText: "Goodbye", nextNode: "exit"},
      hi: {responseText: "Sup", nextNode: "story"},
      what: {responseText: "What???", nextNode: "repeat"},
      first: {responseText: "what did you say first???", nextNode: "entry"},
      getFishStick: {responseText: "Give me a fishstick please!", nextNode: "getFishstick"},
      quest: {responseText: "Do you know where Mayor Shark is?", nextNode: "quest"},
  }
}

// term colors
const FgBlack = "\x1b[30m"
const FgRed = "\x1b[31m"
const FgGreen = "\x1b[32m"
const FgYellow = "\x1b[33m"
const FgBlue = "\x1b[34m"
const FgMagenta = "\x1b[35m"
const FgCyan = "\x1b[36m"
const FgWhite = "\x1b[37m"
const FgGray = "\x1b[90m"

const runConv = async (conv: Conversation) => {
  const currentNode = async (node: DialogueNode) => {
    process.stdout.write(`\n${FgGray}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n`);
    // jank
    if(node.actionType !== "flag") {
      process.stdout.write(` ${FgYellow}${conv.npcName}: ${FgMagenta}${node.text}\n\n`);
    }
    switch (node.actionType) {
      case "response":
        console.log(FgBlue, "Respond:")
        node.responses.forEach((element, index) => {
          console.log(FgGreen,`  ${index}:  ${conv.responseEdges[element].responseText}`);
        });

        const getNextOption = async () => {
          // Jank maybe better safety checking lol, should loop on invalid selection.
          const prompt = `${FgCyan}Choose: `;
          process.stdout.write(prompt);
          for await (const line of console) {
            const selectedNumber = Number(line);
            const selected = conv.responseEdges[node.responses[selectedNumber]];
            if(selected) {
              return selected.nextNode;
            }
            console.log(FgRed, "Invalid Selection ☹");
            process.stdout.write(prompt);
          }
          throw new Error("Unreachable?")
        };
        const nextOption = await getNextOption();
        await currentNode(conv.dialogueNodes[nextOption]);
        return;
      case "getItem":
        const nextNode = conv.dialogueNodes[node.nextNode];
        await currentNode(nextNode);
        return;
      case "exit":
        console.log("Conversation Finished");
        return;
      case "flag":
        console.log(FgYellow, "Flag Triggered: ", node.flag);
        const flagNextNode = conv.dialogueNodes[node.nextNode];
        await currentNode(flagNextNode);
        return;
      default:
        const _exhaustiveCheck: never = node;
        return _exhaustiveCheck;
    }
  }

  await currentNode(conv.dialogueNodes.entry);
};

//await runConv(testConv);
const singleDialog: Conversation = {
  npcName: "1Ed",
  dialogueNodes: {
    entry: {
      actionType: "text",
      text: "I’m Ed! I’m one line of dialog!",
      nextNode: "exit"
    },
    exit: {
      actionType: "exit",
      text: "TODO remove text from exits. Probably remove exits altogether."
    }
  },
  responseEdges: {},
}

const simpleDialog: Conversation = {
  npcName: "Ed",
  dialogueNodes: {
    entry: {
      actionType: "text",
      text: "I’m Ed! I’m the hamfish!",
      nextNode: "line2"
    },
    line2: {
      actionType: "text",
      text: "Boston makes me feel good",
      nextNode: "line3",
    },
    line3: {
      actionType: "text",
      text: "I have a swimmie talkie", nextNode: "exit"
    },
    exit: {
      actionType: "exit",
      text: "TODO remove text from exits. Probably remove exits altogether."
    }
  },
  responseEdges: {},
}

const exportDialogToC = (conv: Conversation) => {
  const dialogMap = Object.values(conv.dialogueNodes).filter(n => n.actionType == "text").map(n => (
    {count: n.text.length, text: n.text }
  )).reduce<any>( //TODO typing
    (acc, curr) => {
      return {
        dialogBuf: acc.dialogBuf + curr.text,
        actionBuf: [...acc.actionBuf, {...curr, offset: acc.dialogBuf.length}]
      }
  },
    {dialogBuf: conv.npcName, actionBuf: []},
  );
  //console.log(JSON.stringify(dialogMap))
  // TODO support multinpc jawns
  const actionString = (action: {offset: number, text: string, count: number}) => `  {ActionDialogLine, {{dialogBuf[${action.offset}], ${action.count}}, {dialogBuf[${0}], ${conv.npcName.length}}, ${0}}}`;

  const file = `
#include "common.h"

char *dialogBuf = "${dialogMap.dialogBuf}";

Action *actionBuf = [
  // {offset, length}
  {ActionEnd, {}},
${dialogMap.actionBuf.map(actionString).join(',\n')}
];
`
  console.log(file)
}

//exportDialogToC(simpleDialog);
exportDialogToC(singleDialog);

//const prompt = "Type something: ";
//process.stdout.write(prompt);
//for await (const line of console) {
//  console.log(`You typed: ${line}`);
//  process.stdout.write(prompt);
//}

//console.log(values);
//console.log(positionals);
