---
title: Forms and inputs
---

# The contract

{{show: form-concept grow 0.5s spring}} A form is a conversation. The page asks questions. The user answers. The browser sends those answers somewhere.

{{focus: user-node}} {{pulse: user-node}} {{annotate: user-node "Fills in data"}} The user types, selects, checks boxes. Every interaction is an answer to a question the form is asking.

{{trace: submit-path}} {{focus: server-node}} {{pulse: server-node}} {{annotate: server-node "Receives data"}} When the user clicks submit, the browser packages every answer into a request and sends it to the server. The server processes it and responds.

{{flow: full-flow}} {{focus: none}} That's a form. Question, answer, submit. Every login page, every checkout, every search bar — they're all this same flow.

{{flow: none}} {{trace: none}}

```diagram:form-concept
user [client]
form [service]
server [service]
user --fills--> form
form --submits--> server
server --responds--> user
---
user-node: user
server-node: server
submit-path: form, server
full-flow: user, form, server
```

# The form element

{{clear: slide}}

{{show: form-anatomy typewriter 1.5s linear}} Every form starts with the `<form>` tag. It's the container that says "everything inside me is part of one submission."

{{focus: action-attr}} {{zoom: 1.3x}} {{pulse: action-attr}} {{annotate: action-attr "Where to send"}} The action attribute is the destination. When the form submits, the browser sends data to this URL.

{{zoom: 1x}} {{focus: method-attr}} {{zoom: 1.2x}} {{pulse: method-attr}} {{annotate: method-attr "How to send"}} The method is how it sends. GET puts data in the URL — good for search. POST puts data in the request body — good for everything else. Use POST for any form that changes data.

{{zoom: 1x}} {{pan: inputs}} {{focus: inputs}} {{zoom: 1.2x}} {{annotate: inputs "The questions"}} Inside the form, inputs ask the questions. Each input has a name — that's the key that arrives at the server. The user's answer is the value.

{{zoom: 1x}} {{pan: submit}} {{focus: submit}} {{zoom: 1.2x}} {{pulse: submit}} {{annotate: submit "Sends the form"}} The submit button fires the submission. Pressing Enter in any text input also submits the form — the browser does this automatically.

{{zoom: 1x}} {{pan: none}} {{focus: none}}

```code:form-anatomy lang=html
<form action="/login" method="POST">

  <label for="email">Email</label>
  <input type="email" id="email" name="email">

  <label for="pass">Password</label>
  <input type="password" id="pass" name="password">

  <button type="submit">Log in</button>

</form>
---
action-attr: 1
method-attr: 1
inputs: 3-7
submit: 9
```

# Labels

{{clear: slide}}

{{split}} {{show: label-code reveal 0.3s}} {{show: label-preview reveal 0.5s spring}} Labels are not optional. They're the single most important accessibility feature of any form.

{{focus: label-for}} {{zoom: 1.2x}} {{pulse: label-for}} {{annotate: label-for "for + id link"}} The `for` attribute on a label must match the `id` on its input. This creates a programmatic link. When a screen reader focuses the input, it reads the label aloud: "Email, text input."

{{zoom: 1x}} {{focus: click-target}} {{annotate: click-target "Click the label"}} Linked labels also expand the click target. Click the word "Email" and the cursor jumps to the input. On mobile, this bigger target area matters.

{{zoom: 1x}} {{focus: no-label}} {{pulse: no-label}} {{annotate: no-label "Broken"}} An input without a label is broken. A screen reader user hears "text input" with no context. They can't tell what to type. Placeholder text is not a label — it disappears when you start typing.

{{focus: none}} {{unsplit}}

```code:label-code lang=html
<!-- Correct: label linked via for/id -->
<label for="email">Email address</label>
<input type="email" id="email" name="email">

<!-- Also correct: wrapping -->
<label>
  Phone number
  <input type="tel" name="phone">
</label>

<!-- Broken: no label at all -->
<input type="text" name="name" placeholder="Name">
---
label-for: 2-3
click-target: 6-9
no-label: 12
```

```preview:label-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; }
  label { display: block; margin-bottom: 4px; font-weight: 500; font-size: 14px; color: #334155; }
  input { display: block; width: 100%; padding: 8px 12px; margin-bottom: 16px; border: 1px solid #cbd5e1; border-radius: 6px; font-size: 16px; box-sizing: border-box; }
  input:focus { outline: 2px solid #3b82f6; outline-offset: 1px; border-color: #3b82f6; }
  .broken { border-color: #ef4444; }
  .note { font-size: 12px; color: #ef4444; margin-top: -12px; margin-bottom: 16px; }
</style>
<label for="demo-email">Email address</label>
<input type="email" id="demo-email" placeholder="you@example.com">
<label>Phone number <input type="tel" placeholder="555-0123"></label>
<input type="text" class="broken" placeholder="Name (no label!)">
<div class="note">Screen reader: "text input" — no context</div>
```

# Input types

{{clear: slide}}

{{show: input-types typewriter 2s linear}} HTML has specialized input types. Each one tells the browser what kind of data to expect — and the browser responds with the right keyboard, validation, and UI.

{{pan: text-types}} {{focus: text-types}} {{zoom: 1.2x}} {{pulse: text-types}} {{annotate: text-types "Text inputs"}} Text, email, password, URL, search, tel. Each triggers a different keyboard on mobile. Email shows the @ key. Tel shows the number pad. URL shows the .com key.

{{zoom: 1x}} {{pan: number-types}} {{focus: number-types}} {{zoom: 1.2x}} {{pulse: number-types}} {{annotate: number-types "Numbers and dates"}} Number shows spinner arrows. Range shows a slider. Date, time, datetime-local — each opens a native date or time picker. No JavaScript library needed.

{{zoom: 1x}} {{pan: choice-types}} {{focus: choice-types}} {{zoom: 1.2x}} {{pulse: choice-types}} {{annotate: choice-types "Choices"}} Checkbox for multiple selections. Radio for single selection from a group. All radios with the same name are linked — selecting one deselects the others.

{{zoom: 1x}} {{pan: special-types}} {{focus: special-types}} {{zoom: 1.2x}} {{annotate: special-types "Special inputs"}} Color opens a color picker. File opens a file browser. Hidden stores data the user doesn't need to see — CSRF tokens, tracking IDs.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Use the right type. The browser validates email format for you when you use `type="email"`. The phone number pad appears for `type="tel"`. The native date picker works for `type="date"`. Free behavior.

```code:input-types lang=html
<!-- Text family -->
<input type="text" name="name">
<input type="email" name="email">
<input type="password" name="pass">
<input type="url" name="website">
<input type="search" name="q">
<input type="tel" name="phone">

<!-- Numbers and dates -->
<input type="number" name="qty" min="1" max="99">
<input type="range" name="vol" min="0" max="100">
<input type="date" name="birthday">
<input type="time" name="alarm">

<!-- Choices -->
<input type="checkbox" name="agree" id="agree">
<input type="radio" name="size" value="sm"> Small
<input type="radio" name="size" value="md"> Medium
<input type="radio" name="size" value="lg"> Large

<!-- Special -->
<input type="color" name="theme">
<input type="file" name="avatar">
<input type="hidden" name="csrf" value="abc123">
---
text-types: 2-7
number-types: 10-13
choice-types: 16-19
special-types: 22-24
```

# Select and textarea

{{clear: slide}}

{{split}} {{show: select-textarea reveal 0.3s}} {{show: select-preview reveal 0.5s spring}} Two more input elements. Each solves a specific problem.

{{focus: select-el}} {{zoom: 1.2x}} {{pulse: select-el}} {{annotate: select-el "Dropdown"}} `<select>` creates a dropdown. Each `<option>` is one choice. The value attribute is what gets submitted — the text between the tags is what the user sees.

{{zoom: 1x}} {{focus: optgroup}} {{pulse: optgroup}} {{annotate: optgroup "Grouped options"}} `<optgroup>` groups related options under a label. Useful when the dropdown has many items that fall into categories.

{{zoom: 1x}} {{focus: textarea-el}} {{zoom: 1.2x}} {{pulse: textarea-el}} {{annotate: textarea-el "Multi-line text"}} `<textarea>` is for multi-line text. Comments, messages, descriptions. The rows attribute sets the initial visible height. The user can type as much as they want.

{{focus: none}} {{unsplit}}

```code:select-textarea lang=html
<label for="country">Country</label>
<select id="country" name="country">
  <option value="">Choose a country</option>
  <optgroup label="North America">
    <option value="us">United States</option>
    <option value="ca">Canada</option>
    <option value="mx">Mexico</option>
  </optgroup>
  <optgroup label="Europe">
    <option value="uk">United Kingdom</option>
    <option value="de">Germany</option>
    <option value="fr">France</option>
  </optgroup>
</select>

<label for="msg">Message</label>
<textarea id="msg" name="message" rows="4"
  placeholder="Write your message..."></textarea>
---
select-el: 2-14
optgroup: 4-8, 9-13
textarea-el: 17-18
```

```preview:select-preview
<style>
  body { font-family: system-ui, sans-serif; padding: 24px; }
  label { display: block; margin-bottom: 4px; font-weight: 500; font-size: 14px; color: #334155; }
  select, textarea { display: block; width: 100%; padding: 8px 12px; margin-bottom: 16px; border: 1px solid #cbd5e1; border-radius: 6px; font-size: 16px; font-family: inherit; box-sizing: border-box; }
  select:focus, textarea:focus { outline: 2px solid #3b82f6; outline-offset: 1px; }
</style>
<label for="d-country">Country</label>
<select id="d-country">
  <option value="">Choose a country</option>
  <optgroup label="North America">
    <option>United States</option><option>Canada</option><option>Mexico</option>
  </optgroup>
  <optgroup label="Europe">
    <option>United Kingdom</option><option>Germany</option><option>France</option>
  </optgroup>
</select>
<label for="d-msg">Message</label>
<textarea id="d-msg" rows="4" placeholder="Write your message..."></textarea>
```

# Validation attributes

{{clear: slide}}

{{show: validation-attrs typewriter 1.5s linear}} HTML has built-in validation. No JavaScript required. The browser checks constraints and shows error messages automatically.

{{focus: required-attr}} {{zoom: 1.3x}} {{pulse: required-attr}} {{annotate: required-attr "Can't be empty"}} `required` means the field must have a value. Submit an empty required field and the browser blocks submission with an error message.

{{zoom: 1x}} {{focus: pattern-attr}} {{zoom: 1.2x}} {{pulse: pattern-attr}} {{annotate: pattern-attr "Regex check"}} `pattern` validates against a regular expression. This one requires exactly five digits — a US zip code. The title attribute becomes the error message.

{{zoom: 1x}} {{pan: minmax-attrs}} {{focus: minmax-attrs}} {{zoom: 1.2x}} {{annotate: minmax-attrs "Range limits"}} `min`, `max`, `minlength`, `maxlength` set numeric and character boundaries. The browser enforces these — the user can't submit out-of-range values.

{{zoom: 1x}} {{pan: none}} {{focus: none}} Built-in validation is the first line of defense. It catches obvious mistakes before the form ever reaches the server. But never trust client-side validation alone — always validate on the server too.

```code:validation-attrs lang=html
<form action="/register" method="POST">

  <!-- required: can't be empty -->
  <label for="name">Name</label>
  <input type="text" id="name" name="name"
    required>

  <!-- type=email + required: must be valid email -->
  <label for="email">Email</label>
  <input type="email" id="email" name="email"
    required>

  <!-- pattern: regex validation -->
  <label for="zip">Zip Code</label>
  <input type="text" id="zip" name="zip"
    pattern="[0-9]{5}"
    title="Five digit zip code">

  <!-- min/max: numeric range -->
  <label for="age">Age</label>
  <input type="number" id="age" name="age"
    min="13" max="120">

  <!-- minlength/maxlength: character limits -->
  <label for="bio">Bio</label>
  <textarea id="bio" name="bio"
    minlength="10" maxlength="500"></textarea>

  <button type="submit">Register</button>
</form>
---
required-attr: 5-6
pattern-attr: 14-16
minmax-attrs: 19-21, 24-25
```

# A complete form

{{clear: slide}}

{{split}} {{show: complete-form reveal 0.3s}} {{show: form-rendered reveal 0.5s spring}} Here's a real form. Every input has a label. Every field has the right type. Validation happens without a line of JavaScript.

{{focus: fieldset}} {{zoom: 1.2x}} {{pulse: fieldset}} {{annotate: fieldset "Group with legend"}} `<fieldset>` groups related inputs. `<legend>` labels the group. Screen readers announce "Personal Information, group" when a user tabs into this section.

{{zoom: 1x}} {{pan: email-field}} {{focus: email-field}} {{pulse: email-field}} {{annotate: email-field "Right type + required"}} Email type with required. The browser validates the format and blocks empty submissions. The autocomplete attribute helps password managers fill it in.

{{zoom: 1x}} {{pan: select-field}} {{focus: select-field}} {{annotate: select-field "Dropdown with default"}} The first option has an empty value and is disabled+selected. It acts as a placeholder that can't be submitted. The required attribute on the select forces a real choice.

{{zoom: 1x}} {{pan: submit-section}} {{focus: submit-section}} {{pulse: submit-section}} {{annotate: submit-section "Submit button"}} The submit button. Type "submit" is explicit — some developers use `<button>` without a type, which defaults to "submit" in a form, but being explicit is clearer.

{{zoom: 1x}} {{pan: none}} {{focus: none}} {{unsplit}}

```code:complete-form lang=html
<form action="/register" method="POST">
  <fieldset>
    <legend>Personal Information</legend>

    <label for="name">Full Name</label>
    <input type="text" id="name" name="name"
      required autocomplete="name">

    <label for="email">Email</label>
    <input type="email" id="email" name="email"
      required autocomplete="email">

    <label for="role">Role</label>
    <select id="role" name="role" required>
      <option value="" disabled selected>
        Choose your role
      </option>
      <option value="dev">Developer</option>
      <option value="design">Designer</option>
      <option value="pm">Product Manager</option>
    </select>

    <label for="bio">Short Bio</label>
    <textarea id="bio" name="bio" rows="3"
      maxlength="300"
      placeholder="Tell us about yourself">
    </textarea>
  </fieldset>

  <button type="submit">Create Account</button>
</form>
---
fieldset: 2-28
email-field: 9-11
select-field: 13-21
submit-section: 30
```

```preview:form-rendered
<style>
  body { font-family: system-ui, sans-serif; padding: 16px; }
  fieldset { border: 1px solid #e2e8f0; border-radius: 8px; padding: 16px; margin: 0; }
  legend { font-weight: 600; font-size: 14px; color: #1e293b; padding: 0 8px; }
  label { display: block; margin-top: 12px; margin-bottom: 4px; font-size: 14px; font-weight: 500; color: #334155; }
  input, select, textarea { display: block; width: 100%; padding: 8px 12px; border: 1px solid #cbd5e1; border-radius: 6px; font-size: 16px; font-family: inherit; box-sizing: border-box; }
  input:focus, select:focus, textarea:focus { outline: 2px solid #3b82f6; outline-offset: 1px; border-color: #3b82f6; }
  button { margin-top: 16px; padding: 10px 20px; background: #3b82f6; color: white; border: none; border-radius: 6px; font-size: 16px; font-weight: 500; cursor: pointer; }
  button:hover { background: #2563eb; }
</style>
<form>
  <fieldset>
    <legend>Personal Information</legend>
    <label for="d-name">Full Name</label>
    <input type="text" id="d-name" required autocomplete="name">
    <label for="d-email">Email</label>
    <input type="email" id="d-email" required autocomplete="email">
    <label for="d-role">Role</label>
    <select id="d-role" required>
      <option value="" disabled selected>Choose your role</option>
      <option>Developer</option><option>Designer</option><option>Product Manager</option>
    </select>
    <label for="d-bio">Short Bio</label>
    <textarea id="d-bio" rows="3" maxlength="300" placeholder="Tell us about yourself"></textarea>
  </fieldset>
  <button type="button">Create Account</button>
</form>
```
