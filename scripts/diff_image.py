"""
Copyright (C) 2025 Xiaomi Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from PIL import Image, ImageChops
import argparse

def main(image1_path, image2_path, output_path, highlight_color):
    try:
        # Open both images
        im1 = Image.open(image1_path)
        im2 = Image.open(image2_path)
    except IOError as e:
        print(f"Failed to open image: {e}")
        return

    # Compare the two images
    diff = ImageChops.difference(im1, im2)

    # Generate a highlighted image of inconsistent pixels
    if diff.getbbox():  # If there are different parts
        # Create an overlay layer with the specified highlight color
        highlight_color_rgba = (*highlight_color, 128)
        highlight_overlay = Image.new("RGBA", im1.size, highlight_color_rgba)

        # Create a mask with 255 only on the difference part
        diff_mask = diff.convert("L")  # Convert the difference image to grayscale
        diff_mask = diff_mask.point(lambda p: p > 0 and 255)  # Create a binary mask

        # Apply the highlight overlay layer to the original image
        combined = Image.composite(highlight_overlay, im1.convert("RGBA"), diff_mask)

        try:
            combined.save(output_path)
            print(f"Saved difference image: {output_path}")
        except IOError as e:
            print(f"Failed to save image: {e}")
    else:
        print(f"No difference: {image1_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare two images and highlight differences.")
    parser.add_argument("-i", "--image1-path", type=str, required=True, help="Path to the first image")
    parser.add_argument("-d", "--image2-path", type=str, required=True, help="Path to the second difference image")
    parser.add_argument("-o", "--output-path", type=str, required=True, help="Path to save the output difference image")
    parser.add_argument("-c", "--highlight-color", type=str, default="(255, 0, 0)", help="Highlight color in RGB format (default: (255, 0, 0))")

    args = parser.parse_args()

    # Convert the highlight color string to a tuple
    try:
        highlight_color = tuple(map(int, args.highlight_color.strip("()").split(",")))
        if len(highlight_color) != 3:
            raise ValueError("Highlight color must be in RGB format with 3 values.")
    except ValueError as e:
        print(f"Invalid highlight color format: {e}")
        exit(1)

    # Call the main function with parsed arguments
    main(args.image1_path, args.image2_path, args.output_path, highlight_color)

