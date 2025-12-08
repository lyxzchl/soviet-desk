Excellent choice. A Soviet Socialist Realism-inspired scene will be visually striking and thematically rich. It lends itself perfectly to dramatic lighting, imposing architecture, and symbolic objects.

Here is a complete project plan for a 3D scene titled **"The Model Worker's Study, 1965"**.

---

### **Project Concept: "The Model Worker's Study, 1965"**
A small, spartan but dignified room of a celebrated Stakhanovite (model worker). It's night. The room is a temple to labor, ideology, and quiet reflection. The scene tells a story of dedication, the state, and the individual.

**Core Story:** The worker has returned from a long shift and a party meeting. He has left his cap and papers on the desk, next to the ever-present portrait of the Leader. The radio plays soft static, waiting for the morning news. The red star outside the window casts a symbolic glow. It's a moment of peaceful, ideological solitude.

---

### **Technical Implementation Plan (Using OpenGL & GLUT/GLFW)**

#### **1. Objects (Minimum 5 - We'll do 7)**
1.  **Desk:** A solid, wooden desk. The central focus.
2.  **Wooden Chair:** Simple, sturdy, pushed slightly back.
3.  **Soviet-era Radio:** On the desk, with simple geometry for speakers and knobs.
4.  **Stack of Books & Pamphlets:** Titled "Программа КПСС" (CPSU Program), "Труд" (Labor).
5.  **Portrait Frame:** On the wall, containing a stylized, idealized portrait (a simple, strong-jawed face or a symbolic hammer/sickle/star).
6.  **Window:** Revealing a small slice of a monumentalist apartment block and a **Red Star** neon sign (a key light source).
7.  **Worker's Cap & Papers:** On the desk, suggesting recent presence.

#### **2. Lighting (Minimum 2 Types)**
*   **Source 1 (Key Light): Diffuse + Specular - The Red Star.** A deep, warm **red directional light** shining through the window. It creates long, dramatic shadows and bathes the room in a symbolic glow. This is the primary "mood" light.
*   **Source 2 (Fill Light): Ambient + Point - The Desk Lamp.** A low-intensity **point light** from a simple desk lamp (modeled or implied) with a slightly warmer (yellowish) tone. It illuminates the desk surface, the books, and the papers, creating a pool of focus.
*   *(Bonus)* **Source 3: Ambient.** Very low global ambient light to faintly see shadowed areas.

**Lighting Interaction:** The user can **toggle the desk lamp (Point Light) on/off**. With it off, the room is lit only by the ominous/symbolic red glow of the state (The Star).

#### **3. Textures (Minimum 3)**
1.  **Wood Grain Texture:** For the desk and chair. Use a repetitive, stained oak or pine texture.
2.  **Plaster Wall Texture:** For the walls. Slightly imperfect, with a matte finish.
3.  **Paper/Book Texture:** For the pamphlets and papers on the desk. Scan or create a yellowed paper texture with faint text.
4.  *(Bonus)* **Fabric Texture:** For the cap (wool) and a simple curtain on the window.

#### **4. Camera Movement**
*   **Default Mode:** A slow, automatic **orbital rotation** around the center of the room, like a contemplative film shot.
*   **Interactive Mode:** User-controlled **FPS-style camera** (WASD + mouse) to walk around the room and examine objects up close.
*   **Interaction:** Pressing key **'C'** will **switch between the two camera modes**.

#### **5. Additional Interaction**
As required, we have two:
1.  **'L' Key:** Toggle the **desk lamp (point light) on/off**.
2.  **'C' Key:** **Switch camera mode** (orbital ↔ FPS).

#### **6. Scene Enhancement (For a Better Grade)**
*   **Simple Animation:** The **Red Star** outside the window pulsates faintly (sinusoidal variation of its light intensity).
*   **Sound:** Optional in OpenGL with aux library. Loop a soft, low hum of radio static or distant factory sounds.
*   **Propaganda Poster:** On the wall, a low-poly relief map or a textured poster with socialist realist art (workers in profile, looking forward).

---

### **Suggested Project Structure**
```
SovietStudy_Project/
├── main.cpp
├── shader.vs / shader.fs (or use fixed pipeline if beginner)
├── textures/
│   ├── wood.jpg
│   ├── plaster.jpg
│   ├── paper.jpg
│   └── red_star_icon.png (for the portrait)
├── models/ (optional .obj files for complex objects like radio)
└── report.pdf
```

### **Implementation Steps**
1.  **Setup:** Initialize OpenGL window (GLFW or GLUT), set up viewport, enable depth testing, lighting, and texture modes.
2.  **Lighting Setup:** Define the three light sources (Red Star - directional, Desk Lamp - point, global ambient). Set their colors (red, warm white, dim grey).
3.  **Create Geometry:** Write functions to draw each object using `GL_QUADS` and `GL_TRIANGLES`. Start with cubes for desk/books, a cylinder for lamp base, a flat quad for portrait/window.
4.  **Apply Textures:** Load textures, bind them, and assign texture coordinates to your objects.
5.  **Camera Logic:** Implement the two camera systems. The orbital camera uses `sin/cos` on a timer for rotation. The FPS camera uses keyboard/mouse input to update `gluLookAt`.
6.  **Interactivity:** Add keyboard callbacks (`glutKeyboardFunc` or GLFW equivalent) for keys 'L' (toggle light 2) and 'C' (switch camera).
7.  **Animation:** In the idle/update function, modify the intensity of the Red Star's light or make its glow pulsate slightly.
8.  **Polish:** Adjust material properties (shininess for the desk, matte for walls), fine-tune light positions and angles for maximum drama.

---

### **Short Report Outline (To Submit)**
**Title:** Project 02: "The Model Worker's Study, 1965" - A Socialist Realist 3D Scene
1.  **Introduction:** Concept, artistic inspiration (Socialist Realism: monumentalism, symbolism, glorification of labor).
2.  **Scene Description & Story:** Describe the room, its objects, and the narrative they create.
3.  **Technical Implementation:**
    *   Objects: List and justification.
    *   Lighting: Description of the Red Star (directional) and Desk Lamp (point). Their symbolic meaning.
    *   Textures: List and purpose.
    *   Camera: Description of the two modes.
    *   Interactions: Which keys and what they do.
4.  **Challenges & Solutions:** (e.g., Managing multiple light sources, implementing camera switching, creating the pulsating star effect).
5.  **Results & Screenshots:** Include images of the scene with both lights on, and with only the red star. Show different camera angles.
6.  **Conclusion:** What you learned, and how the scene achieves the goal of a "coherent, realistic, and aesthetic" environment.
7.  **References:** OpenGL tutorials, texture sources, historical references for Soviet interiors.

---

This plan gives you a strong, thematic foundation that meets all mandatory constraints and offers room for creativity. The red lighting is both a technical requirement (second light type) and a powerful artistic tool.

Start by building the room and the desk, then add lights one by one. Good luck