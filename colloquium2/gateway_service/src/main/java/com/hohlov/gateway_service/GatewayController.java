package com.hohlov.gateway_service;

import io.github.bucket4j.Bucket;
import io.github.bucket4j.ConsumptionProbe;
import jakarta.servlet.http.HttpServletRequest;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.*;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.HttpStatusCodeException;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.servlet.HandlerMapping;

import java.time.Duration;
import java.util.Collections;

@RestController
public class GatewayController {

    private static final Logger log = LoggerFactory.getLogger(GatewayController.class);

    private final Bucket bucket = Bucket.builder()
            .addLimit(limit -> limit.capacity(10)
                    .refillGreedy(10, Duration.ofMinutes(1))).build();

    private final RestTemplate restTemp = new RestTemplate();

    private final String backendAdress = "http://localhost:8081";

    @RequestMapping(value = {"/tasks/**", "/swagger-ui/**", "/v3/api-docs/**", "/swagger-ui.html"})
    public ResponseEntity<?> proxy(@RequestBody(required = false) byte[] body, HttpServletRequest request) {
        ConsumptionProbe probe = bucket.tryConsumeAndReturnRemaining(1);
        if (!probe.isConsumed()) {
            long secondsLeft = probe.getNanosToWaitForRefill() / 1_000_000_000;
            log.warn("Rate limit exceeded for client: {}. Next token in {} sec",
                    request.getRemoteAddr(), secondsLeft);
            return ResponseEntity.status(HttpStatus.TOO_MANY_REQUESTS)
                    .body("Too many requests. Try again in " + secondsLeft + " second(s)");
        }
        String path = String.valueOf(request.getAttribute(HandlerMapping.PATH_WITHIN_HANDLER_MAPPING_ATTRIBUTE));
        String fullAdress = backendAdress + path;
        String method = request.getMethod();
        HttpHeaders headers = new HttpHeaders();
        Collections.list(request.getHeaderNames()).forEach(name -> headers.add(name, request.getHeader(name)));
        HttpEntity<byte[]> entity = new HttpEntity<>(body, headers);
        try {
            ResponseEntity<byte[]> response = restTemp.exchange(fullAdress,
                    HttpMethod.valueOf(method), entity, byte[].class);
            log.info("API: {} {} -> {} [Tokens left: {}]",
                    method, path, response.getStatusCode(), probe.getRemainingTokens());
            if (path != null && path.contains("v3/api-docs") && response.getBody() != null) {
                String json = new String(response.getBody());
                json = json.replace(backendAdress, "http://localhost:8080");
                return ResponseEntity.status(response.getStatusCode())
                        .contentType(MediaType.APPLICATION_JSON)
                        .body(json.getBytes());
            }
            return response;
        } catch (HttpStatusCodeException e) {
            log.error("API error: {} {} -> {}", method, path, e.getStatusCode());
            return ResponseEntity.status(e.getStatusCode())
                    .headers(e.getResponseHeaders())
                    .body(e.getResponseBodyAsByteArray());
        } catch (Exception e) {
            log.error("System error: {} {} -> {}", method, path, e.getMessage());
            return ResponseEntity.status(500).body("Gateway error");
        }
    }
}