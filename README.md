# Inventory System Demonstration: Unreal Engine
This is a project I made in Unreal Engine in C++.

## Feature List
- Items that can be quickly created using Blueprints with C++ capability.
- All derived Items can be picked up by the player and viewed in the Inventory.
- The Inventory dynamically updates whenever a player picks up an Item, displaying the amount and image.
- Inventory can be sorted by Amount and Item Type.

## Difficulties Encountered
- Using multiple different features of the engine in collaboration took more time and research than initially expected. For example, using the new WidgetBlueprint released with Unreal Engine 5 proved to be difficult at first. Yet learning about Component Binding using Unreal's Property System, led to the management of 20 UImages and UTextBlocks to be easy to implement.
- The structure and purpose of later classes in the project became less focused as I moved through the project. I believe this was because of the lack of insight into the classes I did not expect to have, for example, the HUD and Widget. If I were to refactor the code in this project, I would change the Inventory container class to be held within the Widget class in order to consolidate the Inventory-related code.

## Reflection
This project was a fun challenge for me to expose myself to different parts of the engine and the interactions between all of the moving parts. I was hesitant at first to start this project due to it not being a "flashy" video game feature but rather an expected feature. However, creating this project made me realize that paying attention to the details and taking a deep dive into underlying systems within a game can be just as interesting and enjoyable. In the future I'd like to make a more complete game on my own, I'll likely return to this project and make improvements for a full implementation.
