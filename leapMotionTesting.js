const Leap = require('leapjs');
const readline = require('readline');

// Clear console for smooth updating
readline.emitKeypressEvents(process.stdin);
if (process.stdin.isTTY) process.stdin.setRawMode(true);

console.clear();
console.log("Listening for Leap Motion data... Move your hand!");

Leap.loop((frame) => {
    console.clear();  // Clears the screen for updated output

    if (frame.hands.length === 0) {
        console.log("No hands detected. Move your hand into the Leap Motion sensor.");
        return;
    }

    console.log(`Frame ID: ${frame.id}`);
    console.log(`Timestamp: ${frame.timestamp}`);

    frame.hands.forEach((hand, index) => {
        console.log(`\n--- Hand ${index + 1} ---`);
        console.log(`Type: ${hand.type}`);
        console.log(`Palm Position: ${hand.palmPosition}`);
        console.log(`Palm Velocity: ${hand.palmVelocity}`);
        console.log(`Grab Strength: ${hand.grabStrength}`);
        console.log(`Pinch Strength: ${hand.pinchStrength}`);
        console.log(`Sphere Radius: ${hand.sphereRadius}`);
        console.log(`Confidence: ${hand.confidence}`);

        console.log("\nFingers:");
        hand.fingers.forEach((finger) => {
            console.log(`  ${finger.type === 0 ? "Thumb" : finger.type === 1 ? "Index" : finger.type === 2 ? "Middle" : finger.type === 3 ? "Ring" : "Pinky"}:
            - Position: ${finger.tipPosition}
            - Direction: ${finger.direction}
            - Length: ${finger.length}
            - Width: ${finger.width}`);
        });

        console.log("\nGestures:");
        frame.gestures.forEach((gesture) => {
            console.log(`  Type: ${gesture.type}, State: ${gesture.state}`);
        });
    });

    console.log("\nPress 'CTRL + C' to exit.");
});
