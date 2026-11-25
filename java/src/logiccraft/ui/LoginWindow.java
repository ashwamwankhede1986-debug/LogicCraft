package logiccraft.ui;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.*;
import java.security.spec.KeySpec;
import java.util.Base64;
import java.util.Properties;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

/** Local Swing login gate for the Java GUI launcher. Credentials are PBKDF2-hashed. */
public final class LoginWindow {
    private static final Path CREDENTIALS = Paths.get(System.getProperty("user.home"), ".logiccraft", "credentials.properties");
    private static final int ITERATIONS = 150_000;
    private static final int KEY_BITS = 256;
    private LoginWindow() {}

    public static boolean showAndAuthenticate() {
        final boolean firstRun = !Files.exists(CREDENTIALS);
        JTextField user = new JTextField(20);
        JPasswordField pass = new JPasswordField(20);
        JPasswordField confirm = new JPasswordField(20);
        JPanel form = new JPanel(new GridLayout(0, 1, 8, 8));
        form.setBackground(new Color(17, 24, 22));
        form.setBorder(new EmptyBorder(18, 20, 18, 20));
        JLabel heading = new JLabel(firstRun ? "Create your local LogicCraft account" : "Sign in to LogicCraft");
        heading.setForeground(new Color(226, 232, 240));
        heading.setFont(new Font("Segoe UI", Font.BOLD, 16));
        JLabel uLabel = label("Username");
        JLabel pLabel = label("Password");
        form.add(heading); form.add(uLabel); form.add(user); form.add(pLabel); form.add(pass);
        if (firstRun) { form.add(label("Confirm password")); form.add(confirm); }
        style(user); style(pass); style(confirm);
        int result = JOptionPane.showConfirmDialog(null, form, "LogicCraft — Digital Logic Workbench", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
        if (result != JOptionPane.OK_OPTION) return false;
        String username = user.getText().trim();
        char[] password = pass.getPassword();
        char[] confirmation = confirm.getPassword();
        try {
            if (username.isEmpty() || password.length < 8) {
                showError("Enter a username and a password of at least 8 characters."); return false;
            }
            if (firstRun) {
                if (!java.util.Arrays.equals(password, confirmation)) { showError("Passwords do not match."); return false; }
                saveCredential(username, password);
                JOptionPane.showMessageDialog(null, "Local account created. You can now use LogicCraft.", "Setup complete", JOptionPane.INFORMATION_MESSAGE);
                return true;
            }
            if (authenticate(username, password)) return true;
            showError("Invalid username or password."); return false;
        } catch (Exception ex) {
            showError("Login setup failed: " + ex.getMessage()); return false;
        } finally {
            java.util.Arrays.fill(password, '\0');
            java.util.Arrays.fill(confirmation, '\0');
        }
    }

    private static JLabel label(String text) { JLabel l = new JLabel(text); l.setForeground(new Color(148, 163, 184)); return l; }
    private static void style(JComponent c) { c.setBackground(new Color(30, 41, 36)); c.setForeground(new Color(226, 232, 240)); c.setFont(new Font("Segoe UI", Font.PLAIN, 13)); }
    private static void showError(String message) { JOptionPane.showMessageDialog(null, message, "LogicCraft Authentication", JOptionPane.ERROR_MESSAGE); }

    private static void saveCredential(String username, char[] password) throws Exception {
        byte[] salt = new byte[16]; new SecureRandom().nextBytes(salt);
        byte[] hash = derive(password, salt);
        Properties p = new Properties(); p.setProperty("username", username); p.setProperty("salt", Base64.getEncoder().encodeToString(salt)); p.setProperty("hash", Base64.getEncoder().encodeToString(hash)); p.setProperty("iterations", Integer.toString(ITERATIONS));
        Files.createDirectories(CREDENTIALS.getParent());
        try (var out = Files.newOutputStream(CREDENTIALS, StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING, StandardOpenOption.WRITE)) { p.store(out, "LogicCraft local account; password is not stored"); }
    }

    private static boolean authenticate(String username, char[] password) throws Exception {
        Properties p = new Properties(); try (var in = Files.newInputStream(CREDENTIALS)) { p.load(in); }
        if (!username.equals(p.getProperty("username"))) return false;
        byte[] salt = Base64.getDecoder().decode(p.getProperty("salt")); byte[] expected = Base64.getDecoder().decode(p.getProperty("hash"));
        int rounds = Integer.parseInt(p.getProperty("iterations", Integer.toString(ITERATIONS)));
        return MessageDigest.isEqual(expected, derive(password, salt, rounds));
    }

    private static byte[] derive(char[] password, byte[] salt) throws Exception { return derive(password, salt, ITERATIONS); }
    private static byte[] derive(char[] password, byte[] salt, int rounds) throws Exception {
        KeySpec spec = new PBEKeySpec(password, salt, rounds, KEY_BITS);
        try { return SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256").generateSecret(spec).getEncoded(); }
        finally { if (spec instanceof PBEKeySpec) ((PBEKeySpec) spec).clearPassword(); }
    }
}
